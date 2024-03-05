#include "Encoder.h"
// #include Timing.c

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
}  // MEGN540 Lab 3 TODO
static inline bool Right_B()
{
    return bit_is_set( PINF, 0 );
}  // MEGN540 Lab 3 TODO
static inline bool Right_A()
{
    return Right_B() != Right_XOR();  // from slide
}  // MEGN540 Lab 3 TODO

static inline bool Left_XOR()
{
    return bit_is_set( PINB, 4 );
}  // MEGN540 Lab 3 TODO
static inline bool Left_B()
{
    return bit_is_set( PINE, 2 );
}  // MEGN540 Lab 3 TODO
static inline bool Left_A()
{
    return Left_B() != Left_XOR();  // from slide
}  // MEGN540 Lab 3 TODO

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

    SREG |= ( 1 << SREG_I );  // 1
    // SREG |= ( 1 << 7 );

    PCIFR |= ( 1 << PCIF0 );

    EIMSK |= ( 1 << INT6 );  // 2
    DDRE &= ( 0 << PINE6 );
    PORTE &= ( 0 << PINE6 );
    EICRB &= ( 0 << ISC61 );
    EICRB |= ( 0 << ISC60 );  // 3

    DDRF &= ( 0 << PIN0 );
    PORTF &= ( 0 << PINF0 );

    PCMSK0 |= ( 1 << PCINT4 );  // 4
    DDRB &= ( 0 << PIN4 );
    PORTB &= ( 0 << PINB4 );
    PCICR |= ( 1 << PCIE0 );  // 5

    PORTE &= ( 0 << PINE2 );
    DDRE &= ( 0 << PIN2 );

    // right encoder setup
    // SET_BIT( EICRB, ISC60 );  // any logical change on INT6 generates an interrupt request (ISC60 = 1, ISC61 = 0)
    // SET_BIT( EIMSK, INT6 );   // actually turn on INT6 interrupts, idiot

    // // left encoder setup
    // SET_BIT( PCICR, PCIE0 );    // When the PCIE0 bit=1 and the I-bit in the Status Register (SREG)=1, pin change interrupt 0 is enabled.
    // SET_BIT( PCMSK0, PCINT4 );  // enable PCINT4

    // SET_BIT( SREG, 7 );  // enable global interrupts //previously I but errors
    sei();

    // Initialize static file variables. These probably need to be updated.
    // set to currents
    _last_right_A = Right_A();  // maybe leave as zero
    _last_right_B = Right_B();

    _last_left_A   = Left_A();
    _last_left_B   = Left_B();
    _last_left_XOR = Left_XOR();

    _left_counts  = 0;
    _right_counts = 0;
}

/**
 * Function Encoder_Counts_Left returns the number of counts from the left encoder.
 * @return [int32_t] The count number.
 */
int32_t Encoder_Counts_Left()  // from 14.1
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE
    // Note: Interrupts can trigger during a function call and an int32 requires
    // multiple clock cycles to read/save. You may want to stop interrupts, copy the value,
    // and re-enable interrupts to prevent this from corrupting your read/write.
    unsigned char sreg;
    uint32_t temp;

    sreg = SREG;  // save global interrupt flag
    cli();        // disables interrupts

    temp = _left_counts;

    sei();        // restore interrupts
    SREG = sreg;  // restore global interrupt flag

    return temp;
}

/**
 * Function Encoder_Counts_Right returns the number of counts from the right encoder.
 * @return [int32_t] The count number.
 */
int32_t Encoder_Counts_Right()  // counting everytime an interrupt is triggered
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE
    // Note: Interrupts can trigger during a function call and an int32 requires
    // multiple clock cycles to read/save. You may want to stop interrupts, copy the value,
    // and re-enable interrupts to prevent this from corrupting your read/write.
    unsigned char sreg;
    uint32_t temp;

    sreg = SREG;  // save global interrupt flag
    cli();        // disables interrupts

    temp = _right_counts;

    sei();        // restore interrupts
    SREG = sreg;  // restore global interrupt flag

    return temp;
}

/**
 * Function Encoder_Rad_Left returns the number of radians for the left encoder.
 * @return [float] Encoder angle in radians
 */
float Encoder_Rad_Left()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE.  How many counts per rotation???
    return Encoder_Counts_Left() * 0.00109926 * 2 * 3.141592 * 0.5;
}

/**
 * Function Encoder_Rad_Right returns the number of radians for the left encoder.
 * @return [float] Encoder angle in radians
 */
float Encoder_Rad_Right()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE.  How many counts per rotation???
    // 1:75 ratio
    // encoder 12 counts
    // 75.81x12 = 909.7 cpr
    // 1/909.7 = 0.00109926;
    // radians 2*pi
    return Encoder_Counts_Right() * 0.00109926 * 2 * 3.141592 * 0.5;
}

/**
 * Interrupt Service Routine for the left Encoder. Note: May need to check that it is actually PCINT4 that triggered, as
 * the Pin Change Interrupts can trigger for multiple pins.
 * @return
 */
ISR( PCINT0_vect )
{
    bool cur_left_XOR = Left_XOR();
    if( cur_left_XOR != _last_left_XOR ) {
        bool cur_left_A = Left_A();
        bool cur_left_B = Left_B();

        _left_counts += ( cur_left_A ^ _last_left_B ) - ( cur_left_B ^ _last_left_A );

        _last_left_A   = cur_left_A;
        _last_left_B   = cur_left_B;
        _last_left_XOR = cur_left_XOR;
    }
}

/**
 * Interrupt Service Routine for the right Encoder.
 * @return
 */
ISR( INT6_vect )  // INT6 is right
{
    bool cur_right_A = Right_A();
    bool cur_right_B = Right_B();

    _right_counts += ( cur_right_A ^ _last_right_B ) - ( cur_right_B ^ _last_right_A );

    _last_right_A = cur_right_A;
    _last_right_B = cur_right_B;
}