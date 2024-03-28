/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2021.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2021 Andrew Petruska at Colorado School of Mines

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.

*/

#include "Message_Handling.h"

/**
 * Function _Message_Length returns the number of bytes associated with a command string per the
 * class documentation; This is only accessable from this file.
 * @param cmd
 * @return Size of expected string. Returns 0 if unrecognized.
 */
static uint8_t _Message_Length( char cmd );

/**
 * Function Task_Message_Handling processes USB messages as necessary and sets
 * status flags to control the flow of the program.
 */
void Task_Message_Handling( float _time_since_last )
{
    // *** MEGN540  ***
    // YOUR CODE HERE. I suggest you use your peak function and a switch interface
    // Either do the simple stuff strait up, set flags to have it done later.
    // If it just is a USB thing, do it here, if it requires other hardware, do it
    // in the main and set a flag to have it done here.

    // Check to see if their is data in waiting
    if( !USB_Msg_Length() )
        return;  // nothing to process...

    // Get Your command designator without removal so if their are not enough
    // bytes yet, the command persists
    char command = USB_Msg_Peek();
    // char second  = USB_Msg_Get();

    bool command_processed = false;  // /* MEGN540 -- LAB 2 */

    // process command
    switch( command ) {
        case '*':
            if( USB_Msg_Length() >= _Message_Length( '*' ) ) {
                // then process your multiplication...

                // remove the command from the usb recieved buffer using the
                // usb_msg_get() function
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                // Build a meaningful structure to put your data in. Here we want two
                // floats.
                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                // Copy the bytes from the usb receive buffer into our structure so we
                // can use the information
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Call MEGN540_Lab_Task Function
                Multiply_And_Send( data.v1, data.v2 );

                command_processed = true;
            }
            break;
        case '/':
            if( USB_Msg_Length() >= _Message_Length( '/' ) ) {
                // then process your multiplication...

                // remove the command from the usb recieved buffer using the
                // usb_msg_get() function
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                // Build a meaningful structure to put your data in. Here we want two
                // floats.
                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                // Copy the bytes from the usb receive buffer into our structure so we
                // can use the information
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Call MEGN540_Lab_Task Function
                Divide_And_Send( data.v1, data.v2 );

                command_processed = true;
            }
            break;
        case '+':
            if( USB_Msg_Length() >= _Message_Length( '+' ) ) {
                // then process your multiplication...

                // remove the command from the usb recieved buffer using the
                // usb_msg_get() function
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                // Build a meaningful structure to put your data in. Here we want two
                // floats.
                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                // Copy the bytes from the usb receive buffer into our structure so we
                // can use the information
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Call MEGN540_Lab_Task Function
                Add_And_Send( data.v1, data.v2 );

                command_processed = true;
            }
            break;
        case '-':
            if( USB_Msg_Length() >= _Message_Length( '-' ) ) {
                // then process your multiplication...

                // remove the command from the usb recieved buffer using the
                // usb_msg_get() function
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                // Build a meaningful structure to put your data in. Here we want two
                // floats.
                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                // Copy the bytes from the usb receive buffer into our structure so we
                // can use the information
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Call MEGN540_Lab_Task Function
                Subtract_And_Send( data.v1, data.v2 );

                command_processed = true;
            }
            break;
        case '~':
            if( USB_Msg_Length() >= _Message_Length( '~' ) ) {
                // then process your reset by setting the task_restart flag defined in Lab1_Tasks.h
                USB_Msg_Get();
                USB_Send_Byte( 0 );
                Task_Activate( &task_restart, -1 );
                command_processed = true;
            }
        // Return the time it requested followed by the time to complete the action specified by the second imput char.
        case 't':
            if( USB_Msg_Length() >= _Message_Length( 't' ) ) {
                // then process your reset by setting the task_restart flag defined in Lab1_Tasks.h
                USB_Msg_Get();  // removes the first char of the ring buffer
                uint8_t second = USB_Msg_Get();

                // switch for the second input cc
                switch( second ) {
                    case 0:  // Time Now
                        Task_Activate( &task_send_time, -1 );
                        break;

                    case 1:  // Time to complete a full loop iteration
                        Task_Activate( &task_time_loop, -1 );
                        break;

                    default:  // other
                        USB_Send_Msg( "c", '?', 0, 0 );
                        break;
                }

                command_processed = true;
            }
        // Return the time it requested followed by the time to complete the
        // action specified by the second imput char and returns the time every X milliseconds. If the time is zero or negative it canceles the request.
        case 'T':
            if( USB_Msg_Length() >= _Message_Length( 'T' ) ) {
                USB_Msg_Get();
                // then process your reset by setting the task_restart flag defined in Lab1_Tasks.h
                uint8_t second = USB_Msg_Get();

                float timing;
                USB_Msg_Read_Into( &timing, sizeof( timing ) );

                // if( timing <= 0 ) {
                //     Task_Cancel( &task_send_time );
                //     Task_Cancel( &task_time_loop );
                //     command_processed = true;
                //     break;
                // }

                switch( second ) {
                    case 0:  // Time Now
                        Task_Activate( &task_send_time, timing );
                        break;

                    case 1:  // Time to complete a full loop iteration
                        Task_Activate( &task_time_loop, timing );
                        break;

                    default:  // other
                        USB_Send_Msg( "c", '?', 0, 0 );
                        break;
                }

                command_processed = true;
            }
        case 'e':  // return encoder counts for left and right motors
            if( USB_Msg_Length() >= _Message_Length( 'e' ) ) {
                USB_Msg_Get();

                Task_Activate( &task_send_encoders, -1 );

                //
                command_processed = true;
            }
            break;  // need this??
        case 'E':   // return encoder counts for left and right motors every X milliseconds
            if( USB_Msg_Length() >= _Message_Length( 'E' ) ) {
                USB_Msg_Get();

                // data structure
                struct __attribute__( ( __packed__ ) ) {
                    float period;
                } data;

                // get data into structure
                USB_Msg_Read_Into( &data, sizeof( data ) );

                if( data.period > 0 ) {
                    Task_Activate( &task_send_encoders, data.period * 1e-3 );
                } else {
                    Task_Cancel( &task_send_encoders );
                };

                //
                command_processed = true;
            }
            break;
        case 'b':
            if( USB_Msg_Length() >= _Message_Length( 'b' ) ) {
                // then process your reset by setting the task_restart flag defined in Lab1_Tasks.h
                USB_Msg_Get();  // removes the first char of the ring buffer

                Task_Activate( &task_send_battery, -1 );
                command_processed = true;
            }
            break;
        case 'B':  // return current battery voltage level every X milliseconds
            if( USB_Msg_Length() >= _Message_Length( 'B' ) ) {
                USB_Msg_Get();

                // data structure
                struct __attribute__( ( __packed__ ) ) {
                    float period;
                } data;

                // get data into structure
                USB_Msg_Read_Into( &data, sizeof( data ) );

                if( data.period > 0 ) {
                    Task_Activate( &task_send_battery, data.period * 1e-3 );
                } else {
                    Task_Cancel( &task_send_battery );
                };
                //
                command_processed = true;
            }
            break;
            ase 'p':
                // sets pwm command for left and right, if power is acceptable range
                // input chh
                if( USB_Msg_Length() >= _Message_Length( 'p' ) )
            {
                // remove first character the 'p'
                USB_Msg_Get();
                // struct for reading in values
                PWMdata data;
                // read into data struct
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // disable stop task
                Task_Cancel( &task_pwm_stop );
                // set pwm
                Set_Left_Right_PWM( data );

                // Command was processed related to watchdog
                command_processed = true;
            }
            break;
        case 'P':
            // sets pwm command for left and right, if power is acceptable with sign direction
            // input chhf
            if( USB_Msg_Length() >= _Message_Length( 'P' ) ) {
                // remove first character the 'p'
                USB_Msg_Get();
                // struct for reading in values
                PWMdata_t data;
                PWMdata data_pwm;
                // read into data struct
                USB_Msg_Read_Into( &data, sizeof( data ) );
                data_pwm.left  = data.left;
                data_pwm.right = data.right;
                // disable stop task
                Task_Cancel( &task_pwm_stop );

                // set pwm
                Set_Left_Right_PWM( data_pwm );
                // Activate stop task at input time inteval
                Task_Activate( &task_pwm_stop, data.time );

                // Command was processed related to watchdog
                command_processed = true;

                // try task_run with duation
                // Task_Run( &task_pwm_set, data.time );
                // NO this is wrong maybe somethign with task_run( &task( time last ran ) )
                // check time_last_ran and current time
                // maybe something with a task cancel motros then specify time interval, but this would always run will not just run once?

                // uint32_t current_milli = Timing_Get_Milli(); // current
                // data.time; // time for pwm to run for
            }
            break;
        case 's':
            // stops pwm and dissables motors
            if( USB_Msg_Length() >= _Message_Length( 's' ) ) {
                // remove first character the 's'
                USB_Msg_Get();
                // Run stop task once
                Task_Activate( &task_pwm_stop, -1 );

                // Command was processed related to watchdog
                command_processed = true;
            }
            break;
        case 'S':
            // stops pwm and dissables motors
            if( USB_Msg_Length() >= _Message_Length( 'S' ) ) {
                // remove first character the 'S'
                USB_Msg_Get();
                // Run stop task once
                Task_Activate( &task_pwm_stop, -1 );

                // Command was processed related to watchdog
                command_processed = true;
            }
            break;
        case 'q':
            // send system identifiaciton data back to host
            // output time, pwm_l, pwm_r, encoderL, encoderR
            if( USB_Msg_Length() >= _Message_Length( 'q' ) ) {
                // remove first character the 'q'
                USB_Msg_Get();
                // Activate send system id task
                Task_Activate( &task_send_system_id, -1 );

                // Command was processed related to watchdog
                command_processed = true;
            }
            break;
        case 'Q':
            // send sys identifaction at a given interval
            if( USB_Msg_Length() >= _Message_Length( 'Q' ) ) {
                // remove first character the 'Q'
                USB_Msg_Get();
                // Get time interval
                float time_interval;
                USB_Msg_Read_Into( &time_interval, sizeof( time_interval ) );

                // Check if task should cancel or activate
                if( time_interval <= 0 ) {
                    Task_Cancel( &task_send_system_id );
                } else {
                    // Activate send system id task with interval
                    Task_Activate( &task_send_system_id, time_interval );
                }

                // Command was processed related to watchdog
                command_processed = true;
            }
            break;
        default:
            // What to do if you dont recognize the command character
            USB_Send_Byte( '^' );
            // um comment this line if you want to stop the display of each command on top of one another.
            USB_Flush_Input_Buffer();
            break;
    }

    //********* MEGN540 -- LAB 2 ************//
    if( command_processed ) {
        // RESET the WATCHDOG TIMER
        Task_Activate( &task_message_handling_watchdog, 0.0 );
    }
}

/**
 * @brief Function Task_Message_Handling_Watchdog clears the USB recieve (deleting all messages) to flush the buffer if a complete message is not recieved
 * whithin an appropriate amount of time (say 250ms)
 *
 * @param _unused_
 */
void Task_Message_Handling_Watchdog( float _unused_ )
{
    USB_Flush_Input_Buffer();
}

/**
 * Function _Message_Length returns the number of bytes associated with a
 * command string per the class documentation;
 * @param cmd
 * @return Size of expected string. Returns 0 if unreconized.
 */
static uint8_t _Message_Length( char cmd )
{
    switch( cmd ) {
        case '~': return 1; break;
        case '*': return 9; break;
        case '/': return 9; break;
        case '+': return 9; break;
        case '-': return 9; break;
        case 't': return 2; break;
        case 'T': return 6; break;
        case 'e': return 1; break;
        case 'E': return 5; break;
        case 'b': return 1; break;
        case 'B': return 5; break;
        case 'p': return 5; break;
        case 'P': return 9; break;
        case 's': return 1; break;
        case 'S': return 1; break;
        case 'q': return 1; break;
        case 'Q': return 5; break;
        case 'd': return 9; break;
        case 'D': return 13; break;
        case 'v': return 9; break;
        case 'V': return 13; break;
        default: return 0; break;
    }
}
