#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "timer2.h"

#define REG_TRX_STATUS_TRX_STATUS_MASK 0x1F

//extern struct process rest_engine_process;

uint32_t symbol_threshold = 0x00000000;

ISR(SCNT_CMP1_vect)
{
    tot_overflow++;
}
void sleep_init()
{
    ASSR |= _BV(AS2); // enable asynchronous mode, with external oscillator (32.768kHz in our case)

    SCOCR1HH = (symbol_threshold >> 24);
    SCOCR1HL = (symbol_threshold & 0x00ff0000) >> 16;
    SCOCR1LH = (symbol_threshold & 0x0000ff00) >>  8;
    SCOCR1LL = (symbol_threshold & 0x000000ff);
    SCCR0 = _BV(SCEN) ;//| _BV(SCCKSEL);    // enable symbol counter, with TOSC1/2 clock (32.768kHz)
    SCCNTHH = 0x00;
    SCCNTHL = 0x00;
    SCCNTLH = 0x00;
    SCCNTLL = 0x00;

    while (SCSR & _BV(SCBSY));
    SCIRQM = _BV(IRQMCP1);          // enable compare match 1 IRQ*/
}

void sleep_set(uint8_t seconds)
{
    symbol_threshold += (((uint32_t)seconds) * 62500);

    SCOCR1HH = (symbol_threshold >> 24);
    SCOCR1HL = (symbol_threshold & 0x00ff0000) >> 16;
    SCOCR1LH = (symbol_threshold & 0x0000ff00) >>  8;
    SCOCR1LL = (symbol_threshold & 0x000000ff);

    while (SCSR & _BV(SCBSY));
}

void setState(uint8_t state)
{
    TRX_STATE = CMD_FORCE_TRX_OFF;
    TRX_STATE = state;
    while (state != (TRX_STATUS & REG_TRX_STATUS_TRX_STATUS_MASK));
}

void sleep_sparrow()
{
    while(TRX_STATUS_struct.trx_status == BUSY_TX);
    printf("after busy transceiver\n");
    setState(CMD_TRX_OFF);
    TRXPR = 1 << SLPTR;
    PRR1 |= 1 << PRTRX24;
    printf("trx status si trxpr trxcmd %d %d\n", TRX_STATUS, TRXPR);
    //while (TRX_STATUS != 0x0f);
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    cli(); //
    sleep_enable();
    sei(); //
    sleep_cpu();
    sleep_disable();

    TRXPR &= ~(1 << SLPTR);
}

ISR(TIMER2_OVF_vect)
{
  tot_overflow++;
  //process_poll(&rest_engine_process);
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
    while(TRX_STATUS_struct.trx_status == BUSY_TX);
    printf("after busy transceiver\n");
    setState(CMD_TRX_OFF);
    TRXPR = 1 << SLPTR;
    PRR1 |= 1 << PRTRX24;
    printf("trx status si trxpr trxcmd %d %d\n", TRX_STATUS, TRXPR);

    set_sleep_mode(SLEEP_MODE_PWR_SAVE);

    //Initialize counter
    TCNT2 = 0;
    do {} while (ASSR & (1 << TCN2UB));
    sleep_enable();
    sleep_cpu();
    sleep_disable();
    TRXPR &= ~(1 << SLPTR);
}
