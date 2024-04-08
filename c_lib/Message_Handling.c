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
#include "Encoder.h"
#include "MotorPWM.h"
#include "Lab3_Tasks.h"
#include "Lab4_Tasks.h"


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
    // Check to see if their is data in waiting
    if( !USB_Msg_Length() )
        return;  // nothing to process...

    // Look at first message ininput USB 
    char command = USB_Msg_Peek();

    // Command processed related to message watchdog
    bool command_processed = false;

    // process command
    switch( command ) {
        case '*':
            if( USB_Msg_Length() >= _Message_Length( '*' ) ) {
                // Remove the command from the usb recieved buffer using the
                USB_Msg_Get();  

                // Build a meaningful structure to put your data in. Here we want two floats.
                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                // Copy the bytes from the usb receive buffer into our structure so we can use the information
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Call MEGN540_Lab_Task Function
                Multiply_And_Send( data.v1, data.v2 );

                // Command was processed related to watchdog 
                command_processed = true;
            }
            break;
        case '/':
            if( USB_Msg_Length() >= _Message_Length( '/' ) ) {
                // then process your divide...

                USB_Msg_Get(); 

                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                USB_Msg_Read_Into( &data, sizeof( data ) );
                Divide_And_Send( data.v1, data.v2 );

                // /* MEGN540 -- LAB 2 */ 
                command_processed = true;
            }
            break;
        case '+':
            if( USB_Msg_Length() >= _Message_Length( '+' ) ) {
                // then process your plus...

                USB_Msg_Get(); 

                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                USB_Msg_Read_Into( &data, sizeof( data ) );
                Add_And_Send( data.v1, data.v2 );

                // Command was processed related to watchdog 
                command_processed = true;
            }
            break;
        case '-':
            if( USB_Msg_Length() >= _Message_Length( '-' ) ) {
                // then process your minus...

                USB_Msg_Get(); 

                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                USB_Msg_Read_Into( &data, sizeof( data ) );
                Subtract_And_Send( data.v1, data.v2 );

                // Command was processed related to watchdog 
                command_processed = true;
            }
            break;
        case '~':
            if( USB_Msg_Length() >= _Message_Length( '~' ) ) {
                // then process your reset by setting the task_restart flag defined in Lab1_Tasks.h

                // Reset structure message 
                struct __attribute__((__packed__)) {
                    char let[7];
                } msg = { .let = {'R', 'E', 'S', 'E', 'T', ' ', '0'} };

                // Send reset message structure
                USB_Send_Msg("c7s",'~', &msg, sizeof( msg ) );

                // Activate restart task
                Task_ReActivate( &task_restart );

                // Command was processed related to watchdog 
                command_processed = true;
            }
            break;
        case 't': 
            if( USB_Msg_Length() >= _Message_Length( 't' ) ) {
                // remove first character the 't' 
                USB_Msg_Get();
                // remove comand char 
                char command_char = USB_Msg_Get(); 

                // switch depending on command char 
                switch (command_char)
                {
                case '0':
                    // send time now 
                    Task_Activate( &task_send_time, -1.0 ); // execute task once 
                    break;
                case '1':
                    // send loop time
                    Task_Activate( &task_time_loop, -1.0 ); // execute task once 
                    break;
                default:
                    // send ? followed by user command 
                    USB_Send_Msg( "cc", '?', &command_char, sizeof( command_char ) ); 
                    break;
                }
                
                // Command was processed related to watchdog 
                command_processed = true;             
            }
            break; 
        case 'T':  
            if( USB_Msg_Length() >= _Message_Length( 'T' ) ) {
                // return the time of computing the second 'c' every X ms specified by input 
                // remove first character the 't' 
                USB_Msg_Get();
                // remove comand char 
                char command_char = USB_Msg_Get(); 
                float time_interval;
                USB_Msg_Read_Into( &time_interval, sizeof( time_interval ) ); 

                switch ( command_char )
                {
                case '0':
                    if( time_interval > 0 ) {
                        Task_Activate( &task_send_time, time_interval ); // activate on interval
                    } else {
                        Task_Cancel( &task_send_time ); 
                    }
                    break;
                case '1':
                    if( time_interval > 0 ) {
                        Task_Activate( &task_time_loop, time_interval ); // activate on interval
                    } else {
                        Task_Cancel( &task_time_loop ); 
                    }
                    break;
                default:
                    // send ? followed by user command 
                    USB_Send_Msg( "cc", '?', &command_char, sizeof( command_char ) ); 
                    break;
                }

                // Send current time interval while activating tasks or canceling
                USB_Send_Msg( "cf", command_char, &time_interval, sizeof( time_interval ) );

                // Command was processed related to watchdog 
                command_processed = true; 
            }
            break; 
        case 'e':
            // return the encoder counts for the left and right
            if( USB_Msg_Length() >= _Message_Length( 'e' ) ) {
                // remove first character the 'e' 
                USB_Msg_Get();
                // activate to run once 
                Task_Activate( &task_send_encoder_count, -1 );

                // Command was processed related to watchdog 
                command_processed = true;
            }
            break;
        case 'E':
            // return the encoder counts for the left and right at given time interval 
            if( USB_Msg_Length() >= _Message_Length( 'E' ) ) {
                // remove first character the 'e' 
                USB_Msg_Get();
                // pop off time_interval as float 
                float time_interval;
                USB_Msg_Read_Into( &time_interval, sizeof( time_interval ) ); 

                // check if task should cancel 
                if ( time_interval <= 0 ) {
                    Task_Cancel( &task_send_encoder_count ); 
                } else {
                    // activate to run at interval 
                    Task_Activate( &task_send_encoder_count, time_interval );
                }

                // Command was processed related to watchdog 
                command_processed = true;
            }
            break;
        case 'b':
            // return the battery voltage level
            if( USB_Msg_Length() >= _Message_Length( 'b' ) ) {
                // remove first character the 'b' 
                USB_Msg_Get();
                // activate to run once 
                Task_Activate( &task_send_battery_voltage, -1 );

                // Command was processed related to watchdog 
                command_processed = true;
            }
            break;  
        case 'B':
            // return the battery voltage level at given interval 
            if( USB_Msg_Length() >= _Message_Length( 'B' ) ) {
                // remove first character the 'B' 
                USB_Msg_Get();
                // pop off float time interval
                float time_interval;
                USB_Msg_Read_Into( &time_interval, sizeof( time_interval ) ); 

                // check if task should cancel 
                if ( time_interval <= 0 ) {
                    Task_Cancel( &task_send_battery_voltage ); 
                } else {
                    // activate to run at interval
                    Task_Activate( &task_send_battery_voltage, time_interval );
                }

                // Command was processed related to watchdog 
                command_processed = true;
            }
            break;  
        case 'p':
            // sets pwm command for left and right, if power is acceptable range
            // input chh
            if( USB_Msg_Length() >= _Message_Length( 'p' ) ) {
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
        case 'd':
            // specifies the distance to drive (linear followed by angular).
            // input cff
            if( USB_Msg_Length() >= _Message_Length( 'd' ) ) {

                USB_Msg_Get();
                // struct for reading in values
                distanceData data;
                // read into data struct
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Command was processed related to watchdog
                command_processed = true;
            }
            break;
        case 'D':
            // specifies the distance to drive (linear followed by angular), terminates
            // after X milliseconds as specified by the third float. If the third float is
            // negative, the car shall stop.
            // input cfff
            if( USB_Msg_Length() >= _Message_Length( 'D' ) ) {

                USB_Msg_Get();
                // struct for reading in values
                distanceData data;
                // read into data struct
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Command was processed related to watchdog
                command_processed = true;
            }
            break;
        case 'v':
            // specifies the speed to drive (linear followed by angular)
            // input c
            if( USB_Msg_Length() >= _Message_Length( 'v' ) ) {
                USB_Msg_Get();
                // struct for reading in values
                velocityData data;
                // read into data struct
                USB_Msg_Read_Into( &data, sizeof( data ) );

                float left_vel = Left_Displacement(&skid_controller, data.linear, data.angular);
                float right_vel = Right_Displacement(&skid_controller, data.linear, data.angular);
                left_vel = Saturate(left_vel, MotorPWM_Get_Max());
                right_vel = Saturate(right_vel, MotorPWM_Get_Max());

                Controller_Set_Target_Velocity(&skid_controller.controller_left, left_vel);
                Controller_Set_Target_Velocity(&skid_controller.controller_right, right_vel);

                Controller_SetTo(&skid_controller.controller_left, left_vel);
                Controller_SetTo(&skid_controller.controller_right, right_vel);

                MotorPWM_Enable(true);

                MotorPWM_Set_Right(right_vel);
                MotorPWM_Set_Left(left_vel);

                Send_Encoders_velocity(0.0);

                // Command was processed related to watchdog
                command_processed = true;
            }
            break;
        case 'V':
            // Specifies the speed to drive (linear followed by angular), terminates after
            // X milliseconds as specified by the third float. If the third float is negative,
            // the car shall stop.
            // input cf
            if( USB_Msg_Length() >= _Message_Length( 'V' ) ) {

                USB_Msg_Get();
                // struct for reading in values
                velocityData data;
                // read into data struct
                USB_Msg_Read_Into( &data, sizeof( data ) );

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
