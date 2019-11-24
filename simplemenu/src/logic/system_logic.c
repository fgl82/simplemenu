//for soundcard

#include <fcntl.h> //for battery
#include <linux/soundcard.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include "../headers/system_logic.h"
#include "../headers/globals.h"

#ifndef TARGET_PC
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
	write(fd, blank, strlen(blank));
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

	#ifndef TARGET_PC
	Shake_Init();
	device = Shake_Open(0);

	Shake_InitEffect(&effect, SHAKE_EFFECT_RUMBLE);
	effect.u.rumble.strongMagnitude = SHAKE_RUMBLE_STRONG_MAGNITUDE_MAX;
	effect.u.rumble.weakMagnitude = SHAKE_RUMBLE_STRONG_MAGNITUDE_MAX*0.9;
	effect.length = 380;
	effect.delay = 0;
	effect_id = Shake_UploadEffect(device, &effect);

	Shake_InitEffect(&effect1, SHAKE_EFFECT_RUMBLE);
	effect1.u.rumble.strongMagnitude = SHAKE_RUMBLE_STRONG_MAGNITUDE_MAX;
	effect1.u.rumble.weakMagnitude = SHAKE_RUMBLE_STRONG_MAGNITUDE_MAX*0.9;
	effect1.length = 380;
	effect1.delay = 0;
	effect_id1 = Shake_UploadEffect(device, &effect1);

	printf("Uploaded #%d\n", effect_id);
	sleep(3);
//	Shake_Init();
//	if (Shake_NumOfDevices() > 0)
//	{
//		device = Shake_Open(0);
//		Shake_InitEffect(&effect, SHAKE_EFFECT_PERIODIC);
//		effect.u.periodic.waveform		= SHAKE_PERIODIC_SINE;
//		effect.u.periodic.period		= 0.1*0x100;
//		effect.u.periodic.magnitude		= 0x6000;
//		effect.u.periodic.envelope.attackLength	= 0x100;
//		effect.u.periodic.envelope.attackLevel	= 0;
//		effect.u.periodic.envelope.fadeLength	= 0x100;
//		effect.u.periodic.envelope.fadeLevel	= 0;
//		effect.direction			= 0x4000;
//		effect.length				= 2000;
//		effect.delay				= 0;
//		effect_id = Shake_UploadEffect(device, &effect);
//	}
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
//		if (battery_level >= 100) return 5;
//		else if (battery_level > 80) return 4;
//		else if (battery_level > 60) return 3;
//		else if (battery_level > 40) return 2;
//		else if (battery_level > 20) return 1;
		return battery_level;
	}
}
