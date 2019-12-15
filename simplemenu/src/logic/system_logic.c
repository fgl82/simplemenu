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
    if (memdev > 0)
    {
        uint32_t m = mhz / 6;
        memregs[0x10 >> 2] = (m << 24) | 0x090520;
    }
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

uint32_t suspend(uint32_t interval, void *param) {
	if (!isUSBMode) {
		clearTimer();
		oldCPU=currentCPU;
		turnScreenOnOrOff(0);
//		setCPU(OC_SLEEP);
		isSuspended=1;
	} else {
		resetTimeoutTimer();
	}
	return 0;
};

void resetTimeoutTimer() {
	if(isSuspended) {
//		setCPU(oldCPU);
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
	Shake_SimplePeriodic(&effect, SHAKE_PERIODIC_SINE, 0.5, 0.0, 0.15, 0.0);
	effect_id=Shake_UploadEffect(device, &effect);
	#endif
//    uint32_t soundDev = open("/dev/mixer", O_RDWR);
//    int32_t vol = (100 << 8) | 100;
//
//    /* Init memory registers, pretty much required for anthing RS-97 specific */
//    memdev = open("/dev/mem", O_RDWR);
//    if (memdev > 0)
//    {
//        memregs = (uint32_t*)mmap(0, 0x20000, PROT_READ | PROT_WRITE, MAP_SHARED, memdev, 0x10000000);
//        if (memregs == MAP_FAILED)
//        {
//            printf("Could not mmap hardware registers!\n");
//            close(memdev);
//        }
//    }
//
//    /* Setting Volume to max, that will avoid issues, i think */
//    ioctl(soundDev, SOUND_MIXER_WRITE_VOLUME, &vol);
//    close(soundDev);
//
//    /* Set CPU clock to its default */
//    setCPU(OC_NO);
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
	#ifndef TARGET_PC
	FILE *f = fopen("/sys/class/power_supply/usb/online", "r");
	int online;
	fscanf(f, "%i", &online);
	fclose(f);
	if (online) {
		return -1;
	} else {
		FILE *f = fopen("/sys/class/power_supply/battery/capacity", "r");
		int battery_level;
		fscanf(f, "%i", &battery_level);
		fclose(f);
		return battery_level;
	}
	#else
	return -1;
	#endif
}
