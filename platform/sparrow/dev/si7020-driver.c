#include <stdio.h>
#include "dev/si7020-driver.h"

#include <util/delay.h>
#include "i2c.h"

#define SI7020_SLAVE_ADDR 0x40
#define SI7020_RH_CMD 0xE5
#define SI7020_TEMP_CMD 0xE3

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

const struct sensors_sensor si7020_sensor;

void init_uart(uint16_t baudrate){

    uint16_t UBRR_val = (F_CPU/16)/(baudrate-1);

    UBRR0H = UBRR_val >> 8;
    UBRR0L = UBRR_val;

    UCSR0B |= (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0); // UART TX (Transmit - senden) einschalten
    UCSR0C |= (1<<USBS0) | (3<<UCSZ00); //Modus Asynchron 8N1 (8 Datenbits, No Parity, 1 Stopbit)
}

int read_humidity() {
    int ack;
    int ret  = 0;

    PORTE &= ~(1 << PE7);
    DDRE |= (1 << PE7);

    //init_uart(57600);

    _delay_ms(80);
    i2c_init();
    //-------init sensor
    ack = i2c_start(0x80);
    if (ack) {
        printf("error\n");
        i2c_stop();
        PORTE |= (1 << PE7);
        return 2;
    }
    printf("after start\n");
    if (i2c_write(SI7020_TEMP_CMD)) {
        printf("writing command failed\n");
        i2c_stop();
        PORTE |= (1 << PE7);
        return 2;
    }

    printf("after write\n");
    ack = i2c_start(0x81);
    if (ack) {
        printf("error\n");
        i2c_stop();
        PORTE |= (1 << PE7);
        return 2;
    }
    printf("after restart\n");
    ret = ((uint8_t)i2c_read_ack())<<8;
    ret |= i2c_read_nack();
    i2c_stop();

    PORTE |= (1 << PE7);

    return ret;
}

static int value(int type)
{
    return read_humidity();
}

static int configure(int type, int c) {
    return type == SENSORS_ACTIVE;
}

static int status(int type) {
     return 1;
}
SENSORS_SENSOR(si7020_sensor, TEMPERATURE_SENSOR, value, configure, status);
