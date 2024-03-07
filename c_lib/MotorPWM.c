#include "MotorPWM.h"

/**
 * Function Initialize_Motor_PWM initializes the motor PWM on Timer 1 for PWM based voltage control of the motors.
 * The Motor PWM system shall initialize in the disabled state for safety reasons. You should specifically enable
 * Motor PWM outputs only as necessary.
 * @param [uint16_t] MAX_PWM is the maximum PWM value to use. This controls the PWM frequency.
 */
void Initialize_MotorPWM( uint16_t MAX_PWM ) 
{ 
    // Timer1 Compare Output Mode Phase and Frequency Correct PWM 
    // TCCR1A |= ( ( 0 << COM1A1 ) & ( 0 << COM1A0 ) );
    // should be clear or set 

    // Disable motors
    MotorPWM_Enable( false ); 

    // Waveform Generation Mode 8 
    TCCR1A |= ( 1 << WGM10 ); 
    TCCR1B |= ( 1 << WGM13 );

    // Clock Selection 
    // H bridge max pwm 250 kHz 
    // 16 MHz / 64 = 250 kHz 
    TCCR1B |= ( ( 0 << CS12 ) & ( 1 << CS11 ) & ( 1 << CS10 ) ); 

    // TOP selection ICR1 
    // ICR1H 
    // ICR1L 

    // Enable Interrupt Output Compare A, B
    TIMSK1 |= ( ( 1 << OCIE1B ) & ( 1 << OCIE1A ) ); 

    // MAKE SURE TO HAVE A and B 

    // use ICRn for TOP 
    // and then OCRnA for PWM Output 
    // 

    // set pwm resultion ICRn , 
    // counter increments untill ICRn or OCRnA where counter changes direction 
    // OCRnx updated at bottom ensurign equal symmetrical 

    // ok so count up from 0 to Top and down
    // every time you reach a compare match it toggles output 
    // output compare registers OCR1A, OCR1B 

}

/**
 * Function MotorPWM_Enable enables or disables the motor PWM outputs.
 * @param [bool] enable (true set enable, false set disable)
 */
void MotorPWM_Enable( bool enable )
{
    // think it would be relating to the follwoing 10 01 on and 00 off 
    if ( enable ) { 
        // test 10 vs 01 
        TCCR1A |= ( ( 0 << COM1A1 ) & ( 1 << COM1A0 ) );
    } else {
        TCCR1A |= ( ( 0 << COM1A1 ) & ( 0 << COM1A0 ) );
    }
}

/**
 * Function MotorPWM_Is_Enabled returns if the motor PWM is enabled for output.
 * @param [bool] true if enabled, false if disabled
 */
bool MotorPWM_Is_Enabled()
{
    // not sure if correct but possibly 
    bool one = bit_is_set( TCCR1A, COM1A1 ); 
    bool two = bit_is_set( TCCR1A, COM1A0 );

    // if 00 in register 
    if ( !one && !two ) {
        return false;
    } 
    return true; 
}

// /**
//  * Function MotorPWM_Set_Left sets the PWM duty cycle for the left motor.
//  * @return [int32_t] The count number.
//  */
// void MotorPWM_Set_Left( int16_t pwm )
// {

// }

// /**
//  * Function MototPWM_Set_Right sets the PWM duty cycle for the right motor.
//  * @return [int32_t] The count number.
//  */
// void MototPWM_Set_Right( int16_t pwm ) 
// {

// }

// /**
//  * Function MotorPWM_Get_Left returns the current PWM duty cycle for the left motor. If disabled it returns what the
//  * PWM duty cycle would be.
//  * @return [int16_t] duty-cycle for the left motor's pwm
//  */
// int16_t MotorPWM_Get_Left()
// {

// }

// /**
//  * Function MotorPWM_Get_Right returns the current PWM duty cycle for the right motor. If disabled it returns what the
//  * PWM duty cycle would be.
//  * @return [int16_t] duty-cycle for the right motor's pwm
//  */
// int16_t MotorPWM_Get_Right()
// {

// }

/**
 * Function MotorPWM_Get_Max() returns the PWM count that corresponds to 100 percent duty cycle (all on), this is the
 * same as the value written into ICR1 as (TOP).
 */
uint16_t MotorPWM_Get_Max()
{
    char sreg; 
    uint16_t temp_ICR1;
    // save global interrupt flag 
    sreg = SREG; 
    // disable interupts 
    cli(); 
    // coppy value of ICR1
    temp_ICR1 = ICR1; 
    // enable interupts 
    sei(); 
    // restore global interrupt flag 
    SREG = sreg; 

    return temp_ICR1; 
}

/**
 * Function MotorPWM_Set_Max sets the maximum pwm count. This function sets the timer counts to zero because
 * the ICR1 can cause undesired behaviors if change dynamically below the current counts.  See page 128 of the
 * atmega32U4 datasheat.
 */
void MotorPWM_Set_Max( uint16_t MAX_PWM );
