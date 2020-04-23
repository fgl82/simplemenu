#ifndef SYSTEM_LOGIC
#define SYSTEM_LOGIC
void setCPU(uint32_t mhz);
void HW_Init();
void cycleFrequencies();
void resetScreenOffTimer();
void clearTimer();
uint32_t suspend();
void initSuspendTimer();
void setBacklight(int level);
int getBacklight();
void resetScreenOffTimer();
int getBatteryLevel();
void rumble();
#endif
