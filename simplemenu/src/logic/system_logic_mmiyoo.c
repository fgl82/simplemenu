#include <fcntl.h>
#include <linux/soundcard.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include "../headers/logic.h"
#include "../headers/system_logic.h"
#include "../headers/globals.h"
#include "../headers/utils.h"
#include "../headers/cJSON.h"


volatile uint32_t *memregs;
int32_t memdev = 0;
int oldCPU;


typedef struct {
    int channel_value;
    int adc_value;
} SAR_ADC_CONFIG_READ;
 
#define SARADC_IOC_MAGIC                     'a'
#define IOCTL_SAR_INIT                       _IO(SARADC_IOC_MAGIC, 0)
#define IOCTL_SAR_SET_CHANNEL_READ_VALUE     _IO(SARADC_IOC_MAGIC, 1)

static SAR_ADC_CONFIG_READ  adcCfg = {0,0};
static int sar_fd = 0;

static void initADC(void) {
    sar_fd = open("/dev/sar", O_WRONLY);
    ioctl(sar_fd, IOCTL_SAR_INIT, NULL);
}

static int is_charging = 0;
void checkCharging(void) {
    int i = 0;
    FILE *file = fopen("/sys/devices/gpiochip0/gpio/gpio59/value", "r");
    if (file!=NULL) {
        fscanf(file, "%i", &i);
        fclose(file);
    }
    is_charging = i;
}

int percBat = 0;
int firstLaunch = 1; 
int countChecks = 0;

static int checkADC(void) {  
    ioctl(sar_fd, IOCTL_SAR_SET_CHANNEL_READ_VALUE, &adcCfg);

    int old_is_charging = is_charging;
    checkCharging();

        char val[3];

            int percBatTemp = 0;
            if (is_charging == 0) {
                if (adcCfg.adc_value >= 528)
                    percBatTemp = adcCfg.adc_value-478;
                else if ((adcCfg.adc_value >= 512) && (adcCfg.adc_value < 528))
                    percBatTemp = (int)(adcCfg.adc_value*2.125-1068);
                else if ((adcCfg.adc_value >= 480) && (adcCfg.adc_value < 512))
                    percBatTemp = (int)(adcCfg.adc_value* 0.51613 - 243.742);

                if ((firstLaunch == 1) || (old_is_charging == 1)) {
                    // Calibration needed at first launch or when the
                    // user just unplugged his charger
                    firstLaunch = 0;
                    percBat = percBatTemp;
                } else {
                    if (percBat>percBatTemp) {
                        percBat--;
                    } else if (percBat < percBatTemp) {
                        percBat++;
                    }
                }
                if (percBat<0)
                    percBat=0;
                else if (percBat>100)
                    percBat=100;
            } else {
                // The handheld is currently charging
                percBat = 500;
            }
    return percBat;
}

void to_string(char str[], int num) {
    int i, rem, len = 0, n;

    n = num;
    while (n != 0) {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++) {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

void setCPU(uint32_t mhz) {
    currentCPU = mhz;
}

void turnScreenOnOrOff(int state) {
    const char *path = "/sys/class/graphics/fb0/blank";
    const char *blank = state ? "0" : "1";
    int fd = open(path, O_RDWR);
    int ret = write(fd, blank, strlen(blank));
    if (ret==-1) {
        generateError("FATAL ERROR", 1);
    }
    close(fd);
}

void clearTimer() {
    if (timeoutTimer != NULL) {
        SDL_RemoveTimer(timeoutTimer);
    }
    timeoutTimer = NULL;
}

uint32_t suspend() {
    if (timeoutValue!=0&&hdmiEnabled==0) {
        clearTimer();
        oldCPU=currentCPU;
        turnScreenOnOrOff(0);
        isSuspended=1;
    }
    return 0;
};

void resetScreenOffTimer() {
    if (isSuspended) {
        turnScreenOnOrOff(1);
        currentCPU=oldCPU;
        isSuspended=0;
    }
    clearTimer();
    timeoutTimer=SDL_AddTimer(timeoutValue * 1e3, suspend, NULL);
}

void initSuspendTimer() {
    timeoutTimer=SDL_AddTimer(timeoutValue * 1e3, suspend, NULL);
    isSuspended=0;
    logMessage("INFO","initSuspendTimer","Suspend timer initialized");
}

void HW_Init() {
    initADC();
    logMessage("INFO","HW_Init","HW Initialized");
}

void rumble() {

}

int getBatteryLevel() {
    int max_voltage;
    int voltage_now;
    int total;
    int charge = checkADC();
    if (charge<=20)          return 1;
    else if (charge<=40)  return 2;
    else if (charge<=60)  return 3;
    else if (charge<=80)  return 4;
    else if (charge<=100) return 5;
    else                  return 6;
}

char* load_file(char const* path) {
    char* buffer = 0;
    long length = 0;
    FILE * f = fopen (path, "rb"); //was "rb"

    if (f) {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = (char*)malloc ((length+1)*sizeof(char));
        if (buffer) {
            fread (buffer, sizeof(char), length, f);
        }
        fclose (f);
    }
    buffer[length] = '\0';

    return buffer;
}

int getCurrentBrightness() {
    cJSON* request_json = NULL;
    cJSON* itemBrightness;

    const char *request_body = load_file("/appconfigs/system.json");
    request_json = cJSON_Parse(request_body);
    itemBrightness = cJSON_GetObjectItem(request_json, "brightness");
    int dBrightness = cJSON_GetNumberValue(itemBrightness);
    return dBrightness;
}

int getMaxBrightness() {
    return 10;
}

void setBrightness(int value) {
    cJSON* request_json = NULL;
    cJSON* itemBrightness;

    FILE *f = fopen("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", "w");
    if (f!=NULL) {
        fprintf(f, "%d", value * 10);
        fclose(f);
    }

    // Store in system.json
    const char *request_body = load_file("/appconfigs/system.json");
    request_json = cJSON_Parse(request_body);
    itemBrightness = cJSON_GetObjectItem(request_json, "brightness");
    cJSON_SetNumberValue(itemBrightness, value);

    FILE *file = fopen("/appconfigs/system.json", "w");
    char *test = cJSON_Print(request_json);
    fputs(test, file);
    fclose(file);
}

int getCurrentAudioFix() {
    cJSON* request_json = NULL;
    cJSON* itemAudioFix;

    const char *request_body = load_file("/appconfigs/system.json");
    request_json = cJSON_Parse(request_body);
    itemAudioFix = cJSON_GetObjectItem(request_json, "audiofix");
    int dAudioFix = cJSON_GetNumberValue(itemAudioFix);
    return dAudioFix;
}

void setAudioFix(int value) {
    cJSON* request_json = NULL;
    cJSON* itemAudioFix;

    // Store in system.json
    const char *request_body = load_file("/appconfigs/system.json");
    request_json = cJSON_Parse(request_body);
    itemAudioFix = cJSON_GetObjectItem(request_json, "audiofix");
    cJSON_SetNumberValue(itemAudioFix, value);

    FILE *file = fopen("/appconfigs/system.json", "w");
    char *test = cJSON_Print(request_json);
    fputs(test, file);
    fclose(file);
}