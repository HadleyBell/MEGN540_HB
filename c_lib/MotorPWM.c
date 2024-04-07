#include "MotorPWM.h"

/**
 * Function Initialize_Motor_PWM initializes the motor PWM on Timer 1 for PWM based voltage control of the motors.
 * The Motor PWM system shall initialize in the disabled state for safety reasons. You should specifically enable
 * Motor PWM outputs only as necessary.
 * @param [uint16_t] MAX_PWM is the maximum PWM value to use. This controls the PWM frequency.
 */
void Initialize_MotorPWM( uint16_t MAX_PWM ) 
{ 

    // Disable motors
    MotorPWM_Enable( false );

    // Timer1 Compare Output Mode Phase and Frequency Correct PWM A and B 
    TCCR1A |= ( ( 1 << COM1A1 ) | ( 1 << COM1B1 ) );
    // should be clear or set 

    // Waveform Generation Mode 8 PWM, Phase and Frequency ICRn (Top)
    TCCR1B |= ( 1 << WGM13 );

    // Clock Selection 
    // H bridge max pwm 250 kHz 
    // 16 MHz / 64 = 250 kHz 
    // start with no prescallar 
    TCCR1B |= ( 1 << CS10 ); 


    // TOP selection ICR1 from max pwm input
    // ICR1 = MAX_PWM; 
    // Set counting register to 0 
    // TCNT1 = 0; 
    // Sets count 0 and max pwm  
    MotorPWM_Set_Max( MAX_PWM ); 

    // Set output compare registers to 0 OFF
    OCR1A = 0;
    OCR1B = 0; 

    // Enable interupt capture 
    // TIMSK1 |= ( 1 << ICIE1 ); 
    // Enable Interrupt Output Compare A, B
    // TIMSK1 |= ( ( 1 << OCIE1B ) & ( 1 << OCIE1A ) ); 

    // Set data output 0 
    PORTB &= ~( 1 << PORTB5 ) | ~( 1 << PORTB6 ); 



    // DDRB |= ( ( 1 << DDB5 ) | ( 1 << DDB6 ) );
    // MotorPWM_Enable( true ); 
    // MotorPWM_Enable( false ); 



    // testing direction 
    // right PB1 direction
    // left PB2 direction 
    DDRB = ( 1 << DDB1 ) | ( 1 << DDB2 ); 
    // 1 corresponds to reverse and 0 is forward 
    // PORTB |= ( 1 << PB1 ) | ( 1 << PB2 );
    PORTB = ( 0 << PB1 ) | ( 0 << PB2 );



    // testing 
    // DDRB |= ( ( 1 << DDB5 ) | ( 1 << DDB6 ) );
    // inital out value 1 
    // PORTB &= ( ~( 1 << PORTB5 ) & ~( 1 << PORTB6 ) ); 

    // MotorPWM_Enable( true ); 

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
    // // think it would be relating to the follwoing 10 01 on and 00 off 
    // if ( enable ) { 
    //     // test 10 vs 01 
    //     TCCR1A |= ( ( 0 << COM1A1 ) & ( 1 << COM1A0 ) );
    // } else {
    //     TCCR1A |= ( ( 0 << COM1A1 ) & ( 0 << COM1A0 ) );
    // }
    // think it would be relating to the follwoing 10 01 on and 00 off 
    if ( enable ) { 
        // test 10 vs 01 
        DDRB |= ( ( 1 << DDB5 ) | ( 1 << DDB6 ) );
    } else {
        // Data direction input
        DDRB &= ( ~( 1 << COM1A1 ) | ~( 1 << COM1A0 ) );
        // Set data output 0 
        PORTB &= ~( 1 << PORTB5 ) | ~( 1 << PORTB6 ); 
        // Clear output compare registers to 0 OFF
        OCR1A = 0;
        OCR1B = 0; 
    }
}

/**
 * Function MotorPWM_Is_Enabled returns if the motor PWM is enabled for output.
 * @param [bool] true if enabled, false if disabled
 */
bool MotorPWM_Is_Enabled()
{
    // not sure if correct but possibly 
    bool a = bit_is_set( DDRB, DDB5 ); 
    bool b = bit_is_set( DDRB, DDB6 );

    // if 00 in register 
    if ( !a && !b ) {
        return false;
    } 
    return true; 
}

/**
 * Function MotorPWM_Set_Left sets the PWM duty cycle for the left motor.
 * @return [int32_t] The count number.
 */
void MotorPWM_Set_Left( int16_t pwm )
{
    // check direction
    if ( pwm >= 0 ) {
        // forward        
        PORTB &= ~( 1 << PB2 ); 
        OCR1B = pwm; 
    } else {
        // reverse 
        PORTB |= ( 1 << PB2 ); 
        OCR1B = -pwm; 
    }
    // Enable PWM 
    MotorPWM_Enable( true ); 
}

/**
 * Function MotorPWM_Set_Right sets the PWM duty cycle for the right motor.
 * @return [int32_t] The count number.
 */
void MotorPWM_Set_Right( int16_t pwm ) 
{
    // check direction
    if ( pwm >= 0 ) {
        // forward 
        PORTB &= ~( 1 << PB1 ); 
        OCR1A = pwm; 
    } else {
        // reverse 
        PORTB |= ( 1 << PB1 ); 
        OCR1A = -pwm; 
    }
    // Enable PWM 
    MotorPWM_Enable( true ); 
}

/**
 * Function MotorPWM_Get_Left returns the current PWM duty cycle for the left motor. If disabled it returns what the
 * PWM duty cycle would be.
 * @return [int16_t] duty-cycle for the left motor's pwm
 */
int16_t MotorPWM_Get_Left()
{
    // get pwm from register
    int16_t pwm;
    pwm = OCR1B; 

    if ( bit_is_set( PORTB, PB2 ) ) {
        // motor in reverse
        return -pwm;
    } 
    return pwm; 
}

/**
 * Function MotorPWM_Get_Right returns the current PWM duty cycle for the right motor. If disabled it returns what the
 * PWM duty cycle would be.
 * @return [int16_t] duty-cycle for the right motor's pwm
 */
int16_t MotorPWM_Get_Right()
{
    int16_t pwm;
    pwm = OCR1A; 

    if ( bit_is_set( PORTB, PB1 ) ) {
        // motor in reverse
        return -pwm;
    } 
    return pwm; 
}

/**
 * Function MotorPWM_Get_Max() returns the PWM count that corresponds to 100 percent duty cycle (all on), this is the
 * same as the value written into ICR1 as (TOP).
 */
uint16_t MotorPWM_Get_Max()
{
    // char sreg; 
    // uint16_t temp_ICR1;
    // // save global interrupt flag 
    // sreg = SREG; 
    // // disable interupts 
    // cli(); 
    // // coppy value of ICR1
    // temp_ICR1 = ICR1; 
    // // enable interupts 
    // sei(); 
    // // restore global interrupt flag 
    // SREG = sreg; 

    int16_t temp_ICR1;
    temp_ICR1 = ICR1; 
    return temp_ICR1; 
}

/**
 * Function MotorPWM_Set_Max sets the maximum pwm count. This function sets the timer counts to zero because
 * the ICR1 can cause undesired behaviors if change dynamically below the current counts.  See page 128 of the
 * atmega32U4 datasheat.
 */
void MotorPWM_Set_Max( uint16_t MAX_PWM ) 
{
    // Set timer count to 0 
    TCNT1 = 0; 
    // Set new top max PWM 
    ICR1 = MAX_PWM;  

}