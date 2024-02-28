#include "Encoder.h"

/**
 * Internal counters for the Interrupts to increment or decrement as necessary.
 */
static volatile bool _last_right_A = 0;  // Static limits it's use to this file
static volatile bool _last_right_B = 0;  // Static limits it's use to this file

static volatile bool _last_left_A   = 0;  // Static limits it's use to this file
static volatile bool _last_left_B   = 0;  // Static limits it's use to this file
static volatile bool _last_left_XOR = 0;  // Necessary to check if PB4 triggered the ISR or not

static volatile int32_t _left_counts  = 0;  // Static limits it's use to this file
static volatile int32_t _right_counts = 0;  // Static limits it's use to this file

/** Helper Funcions for Accessing Bit Information */
// *** MEGN540 Lab 3 TODO ***
// Hint, use avr's bit_is_set function to help
static inline bool Right_XOR()
{
    return bit_is_set( PINE, 6 );
} 
static inline bool Right_B()
{
    return bit_is_set( PINF, 0 );
}  
static inline bool Right_A()
{
    // reconstructing A from B and XOR, (A XOR B) XOR B = A
    return Right_XOR() ^ Right_B();
}  

static inline bool Left_XOR()
{
    return bit_is_set( PINB, 4 );
}  
static inline bool Left_B()
{
    return bit_is_set( PINE, 2 );
}  
static inline bool Left_A()
{
    // reconstructing A from B and XOR
    return Left_XOR() ^ Left_B();
}  

/**
 * Function Encoders_Init initializes the encoders, sets up the pin change interrupts, and zeros the initial encoder
 * counts.
 */
void Initialize_Encoders()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE

    // Left encoder uses PB4 and PE2 pins as digital inputs. External interrupt PCINT4 is necessary to detect
    // the change in XOR flag. You'll need to see Section 11.1.5 - 11.1.7 for setup and use.
    // Note that the PCINT interrupt is trigered by any PCINT pin. In the ISR you should check to make sure
    // the interrupt triggered is the one you intend on processing.

    // Right encoder uses PE6 adn PF0 as digital inputs. External interrupt INT6 is necessary to detect
    // the changes in XOR flag. You'll need to see Sections 11.1.2-11.1.4 for setup and use.
    // You'll use the INT6_vect ISR flag.

    // Initialize static file variables. These probably need to be updated.
    _last_right_A = 0;  // MEGN540 Lab 3 TODO
    _last_right_B = 0;  // MEGN540 Lab 3 TODO

    _last_left_A   = 0;  // MEGN540 Lab 3 TODO
    _last_left_B   = 0;  // MEGN540 Lab 3 TODO
    _last_left_XOR = 0;  // MEGN540 Lab 3 TODO

    _left_counts  = 0;  // MEGN540 Lab 3 TODO
    _right_counts = 0;  // MEGN540 Lab 3 TODO
}

/**
 * Function Encoder_Counts_Left returns the number of counts from the left encoder.
 * @return [int32_t] The count number.
 */
int32_t Encoder_Counts_Left()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE
    // Note: Interrupts can trigger during a function call and an int32 requires
    // multiple clock cycles to read/save. You may want to stop interrupts, copy the value,
    // and re-enable interrupts to prevent this from corrupting your read/write.
    return 0;
}

/**
 * Function Encoder_Counts_Right returns the number of counts from the right encoder.
 * @return [int32_t] The count number.
 */
int32_t Encoder_Counts_Right()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE
    // Note: Interrupts can trigger during a function call and an int32 requires
    // multiple clock cycles to read/save. You may want to stop interrupts, copy the value,
    // and re-enable interrupts to prevent this from corrupting your read/write.
    return 0;
}

/**
 * Function Encoder_Rad_Left returns the number of radians for the left encoder.
 * @return [float] Encoder angle in radians
 */
float Encoder_Rad_Left()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE.  How many counts per rotation???
    return 0;
}

/**
 * Function Encoder_Rad_Right returns the number of radians for the left encoder.
 * @return [float] Encoder angle in radians
 */
float Encoder_Rad_Right()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE.  How many counts per rotation???
    return 0;
}

/**
 * Interrupt Service Routine for the left Encoder. Note: May need to check that it is actually PCINT4 that triggered, as
 * the Pin Change Interrupts can trigger for multiple pins.
 * @return
 */
ISR( PCINT0_vect )
{
    // // general vector for all PCINT
    // // PCINT4 is on PB4 


    // // check if 
    // if ( Left_XOR() ) {
    //     // execute action 
    //     _last_left_XOR = 1;
    // } else {
    //     _last_left_XOR = 0; 
    // }

    // // get current bool values for a and b from registers
    // bool _current_left_B = Left_B();
    // bool _current_left_A = Left_A();


    // // update left counts based on rotation direction 
    // // A rises before B rises and A falls before B falls p.18
    // _left_counts

    // // could be two different directions 

    // //     For maxon catalog encoder the following definition applies (if not specified otherwise):
    // // If the motor shaft rotates clockwise (CW), channel A leads channel B.
    // // If the motor shaft rotates counter clockwise (CCW), channel B leads channel A.

    // // if XOR triggered means A not same as B 
    // if ( _last_left_XOR == 1 ) {
    //     if ( _last_left_A == 0 && _current_left_A == 1 ) {
    //         if ( _last_left_B == 0 && _current_left_B == 1 ) {

    //         } 
    //         // check here something with direction
    //         // else if ( _last_left_B == 1 && _current_left_B == 0 ) {
                
    //         }
    //     }
    // }


    // // current to last update B, XOR, A for left
    // _last_left_B = _current_left_B;
    // _last_left_XOR = Left_XOR();
    // _last_left_A = _current_left_A;

}

/**
 * Interrupt Service Routine for the right Encoder.
 * @return
 */
ISR( INT6_vect )
{

}