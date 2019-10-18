#ifndef SYSTEM_LOGIC
#define SYSTEM_LOGIC
void setCPU(uint32_t mhz);
void HW_Init();
void cycleFrequencies();
void resetTimeoutTimer();
void clearTimer();
uint32_t suspend(uint32_t interval, void *param);
void initSuspendTimer();
void setBacklight(int level);
int getBacklight();
void resetTimeoutTimer();
uint32_t suspend(uint32_t interval, void *param);
#endif
