/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2023.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2023 Andrew Petruska at Colorado School of Mines

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

*/

#include "Filter.h"
#include <stdio.h>

/**
 * Function Filter_Init initializes the filter given two float arrays and the order of the filter.  Note that the
 * size of the array will be one larger than the order. (First order systems have two coefficients).
 *
 *  1/A_0*( SUM( B_i * input_i )  -   SUM( A_i * output_i) )
 *         i=0..N                    i=1..N
 *
 *  Note a 5-point moving average filter has coefficients:
 *      numerator_coeffs   = { 5 0 0 0 0 };
 *      denominator_coeffs = { 1 1 1 1 1 };
 *      order = 4;
 *
 * @param p_filt pointer to the filter object
 * @param numerator_coeffs The numerator coefficients (B/beta traditionally)
 * @param denominator_coeffs The denominator coefficients (A/alpha traditionally)
 * @param order The filter order
 */
void Filter_Init( Filter_Data_t* p_filt, float* numerator_coeffs, float* denominator_coeffs, uint8_t order )
{
    // initalize all 4 ring buffers set starting index's
    rb_initialize_F( &p_filt->numerator );
    rb_initialize_F( &p_filt->denominator );
    rb_initialize_F( &p_filt->out_list );
    rb_initialize_F( &p_filt->in_list );

    // num coefficents = order + 1
    for ( uint8_t i = 0; i <= order; i++ ) {
        // push back all coefficients 
        rb_push_back_F( &p_filt->numerator, numerator_coeffs[i] );
        rb_push_back_F( &p_filt->denominator, denominator_coeffs[i] );
        // initalize in and out list to 0
        rb_push_back_F( &p_filt->in_list, 0 );
        rb_push_back_F( &p_filt->out_list, 0 );
    }

    return;
}

/**
 * Function Filter_ShiftBy shifts the input list and output list to keep the filter in the same frame. This especially
 * useful when initializing the filter to the current value or handling wrapping/overflow issues.
 * @param p_filt
 * @param shift_amount
 */
void Filter_ShiftBy( Filter_Data_t* p_filt, float shift_amount )
{
    Filter_Data_t temp_shift; 
    rb_initialize_F( &temp_shift.out_list );
    rb_initialize_F( &temp_shift.in_list );

    // pop off in and out list to temp then push back on
    while ( rb_length_F( &p_filt->in_list ) != 0 ) {
        rb_push_back_F( &temp_shift.in_list, rb_pop_front_F( &p_filt->in_list ) );
        rb_push_back_F( &temp_shift.out_list, rb_pop_front_F( &p_filt->out_list ) );
    }

    while ( rb_length_F( &temp_shift.in_list ) != 0 ) {
        rb_push_back_F( &p_filt->in_list, rb_pop_front_F( &temp_shift.in_list ) + shift_amount );
        rb_push_back_F( &p_filt->out_list, rb_pop_front_F( &temp_shift.out_list ) + shift_amount );
    }

    return;
}

/**
 * Function Filter_SetTo sets the initial values for the input and output lists to a constant defined value. This
 * helps to initialize or re-initialize the filter as desired.
 * @param p_filt Pointer to a Filter_Data sturcture
 * @param amount The value to re-initialize the filter to.
 */
void Filter_SetTo( Filter_Data_t* p_filt, float amount )
{
    // length of values in list 
    uint8_t length = rb_length_F( &p_filt->in_list );

    // loop through values and pop old and push new
    for (uint8_t i = 0; i < length; i++)
    {
        rb_pop_front_F( &p_filt->in_list );
        rb_push_back_F( &p_filt->in_list, amount );
        rb_pop_front_F( &p_filt->out_list );
        rb_push_back_F( &p_filt->out_list, amount );
    }
    
    return;
}

/**
 * Function Filter_Value adds a new value to the filter and returns the new output.
 * @param p_filt pointer to the filter object
 * @param value the new measurement or value
 * @return The newly filtered value
 */
float Filter_Value( Filter_Data_t* p_filt, float value )
{
    // length values 
    uint8_t length = rb_length_F( &p_filt->numerator );
    
    // initalize in (b*x) and out sum (a*y) 
    float in_sum = 0;
    float out_sum = 0;

    // pop oldest element from in list 
    rb_pop_front_F( &p_filt->in_list ); 

    // stored num b0, b1...
    // stored den a0, a1...
    // constants a0 and b0, newest in and out
    float b0 = rb_pop_front_F( &p_filt->numerator );
    float a0 = rb_pop_front_F( &p_filt->denominator );
    float in_n = value;
    // don't need to store oldest out
    rb_pop_front_F( &p_filt->out_list ); // pop oldest y_1 where y_n is now
    
    // add the numerator and denominator to back
    rb_push_back_F( &p_filt->numerator, b0 );
    rb_push_back_F( &p_filt->denominator , a0 );

    // compute first in sum as 1 more in sum than out
    in_sum += b0 * in_n; 

    // inputs
    for (uint8_t i = 0; i < length - 1; i++)
    {
        // pop input back and push front go from 1 to N
        float num_b = rb_pop_front_F( &p_filt->numerator );
        float den_a = rb_pop_front_F( &p_filt->denominator ); 
        float in_x = rb_pop_back_F( &p_filt->in_list ); 
        float out_y = rb_pop_back_F( &p_filt->out_list ); 

        rb_push_back_F( &p_filt->numerator, num_b );
        rb_push_back_F( &p_filt->denominator , den_a );
        rb_push_front_F( &p_filt->in_list , in_x );
        rb_push_front_F( &p_filt->out_list , out_y );

        // add to insum and out sum
        in_sum += num_b * in_x;
        out_sum += den_a * out_y;

        // print for troubleshooting 
        // printf( "x, y, %f, %f, %f, %f, %f, %f\n", num_b, den_a, in_x, out_y, in_sum, out_sum );

    }

    // calculate output value 
    float out_val = ( in_sum - out_sum ) / a0;

    // once complete push back newest x and y 
    rb_push_back_F( &p_filt->in_list , in_n );
    rb_push_back_F( &p_filt->out_list , out_val );

    return out_val;
}

/**
 * Function Filter_Last_Output returns the most up-to-date filtered value without updating the filter.
 * @return The latest filtered value
 */
float Filter_Last_Output( Filter_Data_t* p_filt )
{
    // just return the newest lement in out_list, index is length - 1
    return rb_get_F( &p_filt->out_list, rb_length_F( &p_filt->out_list ) - 1);
}

// Function print_rb 
// printing values in a given ring buffer
void print_rb(Ring_Buffer_Float_t* print_f) {
    
    for (uint8_t i = 0; i < rb_length_F( print_f ); i++)
    {
        float te = rb_pop_front_F( print_f );
        printf("%f, ", te );
        rb_push_back_F( print_f, te );
    }
    printf("\n");
}