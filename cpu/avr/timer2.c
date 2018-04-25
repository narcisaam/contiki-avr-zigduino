#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "timer2.h"

extern struct process rest_engine_process;

ISR(TIMER2_OVF_vect)
{
  tot_overflow++;
  process_poll(&rest_engine_process);
}

void timer2_init()
{
    //PRR0 &= ~(1 << PRTIM2);
    tot_overflow = 0;
    // Disable interrupts
    TIMSK2 = 0;
    ASSR = 1 << AS2;
    TIMSK2 = 1 << TOIE2;
    TCCR2A = 0;
    TCCR2B = 0x07; //prescaler 1024 => 8.16 seconds
    do {} while (ASSR & (1 << TCR2BUB));
}

void sparrow_sleep() {
        //Disable transciever
        TRXPR = 1 << SLPTR;

        set_sleep_mode(SLEEP_MODE_PWR_SAVE);

        //Initialize counter
        TCNT2 = 0;
        do {} while (ASSR & (1 << TCN2UB));
        sleep_enable();
        sleep_cpu();
        sleep_disable();
}
