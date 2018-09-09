#ifndef __TIMER2_H__
#define __TIMER2_H__

volatile uint8_t tot_overflow;

void timer2_init();
void sparrow_sleep();
void ZigduinoSleepInit();
void ZigduinoSleepSet(uint8_t seconds);
void ZigduinoSleep();

#endif
