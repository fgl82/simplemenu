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
#ifdef TARGET_RG350
#include <shake.h>
#endif


volatile uint32_t *memregs;
int32_t memdev = 0;
int oldCPU;

void setCPU(uint32_t mhz)
{
	currentCPU = mhz;
	#ifndef TARGET_PC
    if (memdev > 0)
    {
        uint32_t m = mhz / 6;
        memregs[0x10 >> 2] = (m << 24) | 0x090520;
    }
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
	#ifdef TARGET_RG350
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
	#ifdef TARGET_RG350_BETA

		FILE *f = fopen("/sys/class/power_supply/jz-battery/voltage_max_design", "r");
		int max_voltage;
		fscanf(f, "%i", &max_voltage);
		fclose(f);

		f = fopen("/sys/class/power_supply/jz-battery/voltage_min_design", "r");
		int min_voltage;
		fscanf(f, "%i", &min_voltage);
		fclose(f);

		f = fopen("/sys/class/power_supply/jz-battery/voltage_now", "r");
		int voltage_now;
		fscanf(f, "%i", &voltage_now);
		fclose(f);

		int total = ((voltage_now-min_voltage)*100)/(max_voltage-min_voltage);
		if (total > 100 ) {
			return 100;
		}
		return total;

	#elif TARGET_PC
		return 100;
	#else
		return -1;
	#endif
}
