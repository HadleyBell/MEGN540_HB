#include "Encoder.h"
#include "../c_lib/SerialIO.h"


// define counts per rad from 75.81 × 12 ≈ 909.7 CPR
#define _count_per_rad 75.81 * 12 * 0.5 / 3.14159

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

    cli();


    // global interupt enable, MIHGT ALREADY BE  
    SREG |= ( 1 << 7 );

    // interupt control, trigger on falling and rising
    EICRB |= ( ( 0 << ISC61 ) | ( 1 << ISC60 ) );

    // set left B channel, Port E 2 as input 
    DDRE |= ( 0 << DDE2 );
    // set left XOR channel, Port B 4 as input 
    DDRB |= ( 0 << DDB4 );
    // set left PCIE interupt control enabled
    PCICR |= ( 1 << PCIE0 );
    // set left PCIF interupt flag enabled
    PCIFR |= ( 1 << PCIF0 ); 
    // set left interupt, Port B 4  
    PCMSK0 |= ( 1 << PCINT4 ); 

    
    // check on if the rising or falling ege should generate interupt ? what should it be  

    // // working 
    // // set right B channel, Port F 0 as output 
    // PORTF |= ( 1 << PORTF0 ); 
    // // set right XOR channel, Port E 6 as output 
    // PORTE |= ( 1 << PORTE6 ); 
    // // set right interupt, Port E 6 
    // EIMSK |= ( 1 << INT6 ); 

    // messed this up WORKING
    // set right B channel, Port F 0 as input 
    DDRF |= ( 0 << DDF0 ); 
    // set right XOR channel, Port E 6 as input 
    DDRE |= ( 0 << PORTE6 ); 
    // set right interupt, Port E 6 
    EIMSK |= ( 1 << INT6 ); 

    // // interupt control, trigger on falling and rising
    // EICRB |= ( ( 1 << ISC61 ) | ( 1 << ISC60 ) );

    // CHECK IS IT NORMAL FOR USB TO DISCOONECT IF INTERUPT IS ENABLED ?? 
    // CAN REINITALIZE IT AT AN INTERVAL 

    // Initialize static file variables. These probably need to be updated.
    _last_right_A = 0;  // MEGN540 Lab 3 TODO
    _last_right_B = 0;  // MEGN540 Lab 3 TODO

    _last_left_A   = 0;  // MEGN540 Lab 3 TODO
    _last_left_B   = 0;  // MEGN540 Lab 3 TODO
    _last_left_XOR = 0;  // MEGN540 Lab 3 TODO

    _left_counts  = 0;  // MEGN540 Lab 3 TODO
    _right_counts = 0;  // MEGN540 Lab 3 TODO

    // enable interupts 
    sei();
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
    
    // stop interupts 
    cli();

    // coppy value 
    int32_t temp_left = _left_counts; 

    // enable interupts
    sei();

    return temp_left;
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

    // stop interupts 
    cli();

    // coppy value 
    int32_t temp_right = _right_counts; 

    // enable interupts
    sei();

    return temp_right;
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

    return _right_counts / _count_per_rad;
}

/**
 * Interrupt Service Routine for the left Encoder. Note: May need to check that it is actually PCINT4 that triggered, as
 * the Pin Change Interrupts can trigger for multiple pins.
 * @return
 */
ISR( PCINT0_vect )
{
    // disable interups
    cli(); 

    // // general vector for all PCINT
    // // PCINT4 is on PB4 

    // can check PCIFR, PCIF0 if 1 is set 

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

    // check Port B 4 to see if the interupt is from this input 

    // use lat_left_XOR in comparison with current_left_xor 

    bool _current_left_XOR = Left_XOR();

    // compare current with last if changed than execute 
    // this determines if interupt was PCINT4
    if ( _current_left_XOR != _last_left_XOR ) {
        // check direction
        uint8_t dir_fun = Motor_Direction( _last_left_A, _last_left_B, Left_A, Left_B );
        if ( dir_fun == 0 ) {
            // increment left_counts 
            _left_counts++;
        } else if ( dir_fun == 1 ) {
            // decrement left_counts 
            _left_counts--;
        } 


        // // printing for error checking  
        // struct __attribute__( ( __packed__ ) ) {
        //     bool t1;
        //     bool t2;
        //     bool t3;
        //     bool t4;
        //     bool t5;
        // } data;
        // data.t1 = _last_left_A;
        // data.t2 = _last_left_B;
        // data.t3 = Left_A();
        // data.t4 = Left_B();
        // data.t5 = Left_XOR();
        // USB_Send_Msg( "cBBBBB",'1', &data, sizeof( data ) );
        
    
        // update last 
        _last_left_A = Left_A();
        _last_left_B = Left_B();
        _last_left_XOR = Left_XOR();
    } 

    // enable interupts 
    sei();
}

/**
 * Interrupt Service Routine for the right Encoder.
 * @return
 */
ISR( INT6_vect )
{
    // disable interups
    cli(); 

    // char b = 'R';
    // USB_Send_Msg( "cc",'t', &b, sizeof( b ) );

    uint8_t dir_fun = Motor_Direction( _last_right_A, _last_right_B, Right_A, Right_B );

    // check direction 
    if ( dir_fun == 0 ) {
        // increment right_counts 
        _right_counts++;
    } else if ( dir_fun == 1 ) {
        // decrement right_counts 
        _right_counts--;
    } 

    // sending 
    // // float 
    // uint32_t temp = _right_counts;
    // // send USB message for testing 

    // printing for error checking 
    /* 
    struct __attribute__( ( __packed__ ) ) {
        bool t1;
        bool t2;
        bool t3;
        bool t4;
        bool t5;
    } data;
    data.t1 = _last_right_A;
    data.t2 = _last_right_B;
    data.t3 = Right_A();
    data.t4 = Right_B();
    data.t5 = Right_XOR();
    USB_Send_Msg( "cBBBBB",'1', &data, sizeof( data ) );
    */

    // update last 
    _last_right_A = Right_A();
    _last_right_B = Right_B();

    // enable interupts 
    sei();
}


/**
 * Function that determines motor direction from encoder
 * @param last_a prior A channel 
 * @param last_b prior B channel 
 * @param current_a current A channel 
 * @param current_b current B channel
 * @return uint8_t  0 if  Forward CCW, 1 if Reverse CW, and 2 if else 
 */
uint8_t Motor_Direction( bool last_a, bool last_b, bool (*current_a_fun)(), bool (*current_b_fun)() ) {
    bool current_a = current_a_fun();
    bool current_b = current_b_fun();

    if ( ( last_a == 0 && last_b == 0 && current_a == 1 && current_b == 0 ) ||
        ( last_a == 1 && last_b == 0 && current_a == 1 && current_b == 1 ) ||
        ( last_a == 1 && last_b == 1 && current_a == 0 && current_b == 1 ) ||
        ( last_a == 0 && last_b == 1 && current_a == 0 && current_b == 0 ) ) {
            return 0;
    } else if ( ( last_a == 0 && last_b == 0 && current_a == 0 && current_b == 1 ) ||
        ( last_a == 0 && last_b == 1 && current_a == 1 && current_b == 1 ) ||
        ( last_a == 1 && last_b == 1 && current_a == 1 && current_b == 0 ) ||
        ( last_a == 1 && last_b == 0 && current_a == 0 && current_b == 0 ) ) {
            return 1;
    } else {
        return 2; 
    }

}