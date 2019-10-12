//for soundcard

#include <fcntl.h> //for battery
#include <linux/soundcard.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#include "../headers/definitions.h"
#include "../headers/globals.h"

volatile uint32_t *memregs;
int32_t memdev = 0;

void setCPU(uint32_t mhz)
{
	currentCPU = mhz;
    if (memdev > 0)
    {
        uint32_t m = mhz / 6;
        memregs[0x10 >> 2] = (m << 24) | 0x090520;
    }
}

void HW_Init()
{
    uint32_t soundDev = open("/dev/mixer", O_RDWR);
    int32_t vol = (100 << 8) | 100;

    /* Init memory registers, pretty much required for anthing RS-97 specific */
    memdev = open("/dev/mem", O_RDWR);
    if (memdev > 0)
    {
        memregs = (uint32_t*)mmap(0, 0x20000, PROT_READ | PROT_WRITE, MAP_SHARED, memdev, 0x10000000);
        if (memregs == MAP_FAILED)
        {
            printf("Could not mmap hardware registers!\n");
            close(memdev);
        }
    }

    /* Setting Volume to max, that will avoid issues, i think */
    ioctl(soundDev, SOUND_MIXER_WRITE_VOLUME, &vol);
    close(soundDev);

    /* Set CPU clock to its default */
    setCPU(MED_OC);
}

void cycleFrequencies() {
	if(currentCPU==NO_OC) {
		setCPU(MED_OC);
	} else if (currentCPU==MED_OC) {
		setCPU(MAX_OC);
	} else {
		setCPU(NO_OC);
	}
}
