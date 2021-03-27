//for soundcard

#include <fcntl.h> //for battery
#include <linux/soundcard.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include "../headers/logic.h"
#include "../headers/system_logic.h"
#include "../headers/globals.h"
#if defined TARGET_RG350 || defined TARGET_RG350_BETA
#include <shake.h>
#endif

#if defined TARGET_RG350_BETA
#define SYSFS_CPUFREQ_DIR "/sys/devices/system/cpu/cpu0/cpufreq"
#define SYSFS_CPUFREQ_LIST SYSFS_CPUFREQ_DIR "/scaling_available_frequencies"
#define SYSFS_CPUFREQ_SET SYSFS_CPUFREQ_DIR "/scaling_setspeed"
#define SYSFS_CPUFREQ_CUR SYSFS_CPUFREQ_DIR "/scaling_cur_freq"
#endif

volatile uint32_t *memregs;
int32_t memdev = 0;
int oldCPU;

void to_string(char str[], int num)
{
    int i, rem, len = 0, n;

    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

void setCPU(uint32_t mhz)
{
	currentCPU = mhz;
	#ifndef TARGET_PC
		#if defined TARGET_RG350_BETA
			char strMhz[10];
			int fd = open(SYSFS_CPUFREQ_SET, O_RDWR);
			to_string(strMhz, (mhz * 1000));
			int ret = write(fd, strMhz, strlen(strMhz));
			close(fd);
		#endif
	#endif
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
	if(timeoutValue!=0&&hdmiEnabled==0) {
		if (!isUSBMode) {
			clearTimer();
			oldCPU=currentCPU;
			turnScreenOnOrOff(0);
			isSuspended=1;
		} else {
			resetScreenOffTimer();
		}
	}
	return 0;
};

void resetScreenOffTimer() {
	if(isSuspended) {
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
}

void HW_Init()
{
	#if defined TARGET_RG350 || defined TARGET_RG350_BETA
	Shake_Init();
	device = Shake_Open(0);
	Shake_SimplePeriodic(&effect, SHAKE_PERIODIC_SQUARE, 0.5, 0.1, 0.05, 0.1);
	Shake_SimplePeriodic(&effect1, SHAKE_PERIODIC_SQUARE, 0.5, 0.1, 0.05, 0.1);
	effect_id=Shake_UploadEffect(device, &effect);
	effect_id1=Shake_UploadEffect(device, &effect1);
	#endif
}

void cycleFrequencies() {
	if(currentCPU==OC_UC) {
		setCPU(OC_NO);
	} else if (currentCPU==OC_NO) {
		setCPU(OC_OC);
	} else {
		setCPU(OC_UC);
	}
}

void rumble() {

}

int getBatteryLevel() {
	int max_voltage;
	int min_voltage;
	int voltage_now;
	int total;
	#if defined TARGET_RG350_BETA
		FILE *f = fopen("/sys/class/power_supply/jz-battery/voltage_max_design", "r");
		fscanf(f, "%i", &max_voltage);
		fclose(f);

		f = fopen("/sys/class/power_supply/jz-battery/voltage_min_design", "r");
		fscanf(f, "%i", &min_voltage);
		fclose(f);

		f = fopen("/sys/class/power_supply/jz-battery/voltage_now", "r");
		fscanf(f, "%i", &voltage_now);
		fclose(f);

		total = ((voltage_now-min_voltage)*100)/(max_voltage-min_voltage);
		if (total > 100 ) {
			return 100;
		}
		return total;
	#elif TARGET_RG350
		FILE *f = fopen("/sys/class/power_supply/battery/voltage_max_design", "r");
		fscanf(f, "%i", &max_voltage);
		fclose(f);

		f = fopen("/sys/class/power_supply/battery/voltage_min_design", "r");
		fscanf(f, "%i", &min_voltage);
		fclose(f);

		f = fopen("/sys/class/power_supply/battery/voltage_now", "r");
		fscanf(f, "%i", &voltage_now);
		fclose(f);

		total = ((voltage_now-min_voltage)*100)/(max_voltage-min_voltage);
		if (total > 100 ) {
			return 100;
		}
		return total;
	#else
		FILE *f = fopen("/sys/class/power_supply/BAT0/charge_full", "r");
		fscanf(f, "%i", &max_voltage);
		fclose(f);

		f = fopen("/sys/class/power_supply/BAT0/charge_now", "r");
		fscanf(f, "%i", &voltage_now);
		fclose(f);

		total = (voltage_now*100)/(max_voltage);
		if (total > 100 ) {
			return 100;
		}
		return total;
	#endif
}
