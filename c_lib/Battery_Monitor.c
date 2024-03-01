#include "Battery_Monitor.h"
#include "Filter.h"


static const float BITS_TO_BATTERY_VOLTS = 0.00488758553275;

/**
 * Function Initialize_Battery_Monitor initializes the Battery Monitor to record the current battery voltages.
 */
void Initialize_Battery_Monitor()
{

    // *** MEGN540 LAB3 YOUR CODE HERE ***

    // Enable ADC 
    ADCSRA |= ( 1 << ADEN ); 

    // Check on Auto Trigger Enable 

    // Set 128 prescallar 
    ADCSRA |= ( ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 ) ); 

    // Battery connected to PF6, ADC
    
    // so in read do you get start ADC ADSC, ADCSRA  

    // Reference selection Interal 2.56V as voltage divided / 2 with cap AREF AREF 
    // ADMUX &= ~( 1 << REFS1 ) & ( 1 << REFS0 );
    // // Reference selection Interal 2.56V as voltage divided / 2 with cap AREF AREF 
    // ADMUX |= ( ( 0 << REFS1 ) | ( 0 << REFS0 ) );
    ADMUX |= ( ( 1 << REFS1 ) | ( 1 << REFS0 ) );

    // Setup ADC right adjusted, clear to 0 
    ADMUX &= ~( 1 << ADLAR );

    // Disable auto trigger 
    ADCSRA &= ~(1 << ADATE);

    // Specify ADC6
    ADMUX |= ( ( 0 << MUX4 ) | ( 0 << MUX3 ) | ( 1 << MUX2 ) | ( 1 << MUX1 ) | ( 0 << MUX0 ) ); 


    // // testing other 
    // ADCSRA |= ( 1 << ADEN ); 
    // ADCSRA |= ( ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 ) ); 
    // // ADMUX |= ( 1 << REFS0 ); 
    // ADMUX |= ( ( 1 << REFS1 ) | ( 1 << REFS0 ) );

    // LEFT vs. Right adjusted 

    // initalize filter 
    float a_coeff[] = { 1, 0, 0, 0, 0 };
    float b_coeff[] = { 0.0354, 0.2409, 0.4473, 0.2409, 0.0354 };
    Filter_Init( &Voltage_Filter, b_coeff, a_coeff, sizeof( a_coeff ) / sizeof( float ) - 1 );

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

    // start a converstion 
    ADCSRA |= ( 1 << ADSC ); // pretty sure it should be this bit 6 not 1 

    // wait untill conversion is complete 
    while ( bit_is_set( ADCSRA, ADSC ) ) {
        // wait untill the ACD complete bit is set 
    }

    // read the values from the registers 
    data.split.LSB = ADCL; 
    data.split.MSB = ADCH; 

    // add newest data to filter 
    float newest_in_val = (uint16_t)( ( data.split.MSB << 8 ) | data.split.LSB ); 
    // float newest_in_val = (uint16_t)( data.split.LSB ); 
    // data.value = Filter_Value( &Voltage_Filter, newest_in_val ); 
    data.value = Filter_Value( &Voltage_Filter, newest_in_val * BITS_TO_BATTERY_VOLTS ); 

    // // update data.value 
    // data.value = (uint16_t)( ( data.split.MSB << 8 ) | data.split.LSB ); 

    // set conversion to finished clear interupt, not sure if needed as interupts not used 
    ADCSRA |= ( 1 << ADIF ); // pretty sure it should be this bit 6 not 1 


    // think something is slightly wrong here but need to see 


    return data.value * BITS_TO_BATTERY_VOLTS;
}

/**
 * Function Battery_Recent returns most recent voltage post filter 
 */
float Battery_Recent() { 
    return Filter_Last_Output( &Voltage_Filter );
}

// Bat Value 
// 1022.8976440429688]
// appeas it could be max of 10-bit ADC 1023 