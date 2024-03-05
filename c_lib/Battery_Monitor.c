#include "Battery_Monitor.h"

#include "Encoder.h"

static const float BITS_TO_BATTERY_VOLTS = 0.005004888;
// vref = 2.56
// 2*ADC*2.56/1023

/**
 * Function Initialize_Battery_Monitor initializes the Battery Monitor to record the current battery voltages.
 */
void Initialize_Battery_Monitor()
{

    // voltage reference selection??? probably internal 2.56V
    ADMUX |= ( 1 << REFS0 );
    ADMUX |= ( 1 << REFS1 );

    // The ADC is enabled by setting the ADC Enable bit, ADEN in ADCSRA.
    ADCSRA |= ( 1 << ADEN );

    // for 128 division factor table 24-5
    // set prescaler to 128 (1, 1, 1)
    ADCSRA |= ( 1 << ADPS2 );
    ADCSRA |= ( 1 << ADPS1 );
    ADCSRA |= ( 1 << ADPS0 );

    // need to select input channel, analog pin 1 (PF6) (ADC6/TDO)
    // MUX(5:0) = (000110)
    ADMUX |= ( 1 << MUX1 );
    ADMUX |= ( 1 << MUX2 );

    ADCSRB &= ( 0 << MUX5 );
}

/**
 * Function Battery_Voltage initiates the A/D measurement and returns the result for the battery voltage.
 */
float Battery_Voltage()
{
    // A Union to assist with reading the LSB and MSB in the  16 bit register
    union {
        struct {
            uint8_t LSB;
            uint8_t MSB;
        } split;
        uint16_t value;
    } data = { .value = 0 };

    // *** MEGN540 LAB3 YOUR CODE HERE ***
    ADCSRA |= ( 1 << ADSC );                // starts conversion
    while( bit_is_set( ADCSRA, ADSC ) ) {}  // wait for conversion to complete

    // these together are ADC
    data.split.LSB = ADCL;
    data.split.MSB = ADCH & 0b00000011;

    return data.value * BITS_TO_BATTERY_VOLTS;
}
