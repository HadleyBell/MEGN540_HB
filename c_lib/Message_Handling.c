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

    // USB_Send_Byte( 2 );


    // rb_push_back_B(&_usb_send_buffer, 1);

    // Check to see if their is data in waiting
    if( !USB_Msg_Length() )
        return;  // nothing to process...

    // Get Your command designator without removal so if their are not enough
    // bytes yet, the command persists

    char command = USB_Msg_Peek();

    // /* MEGN540 -- LAB 2 */ 
    bool command_processed = false;

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

                // interpret bit structure
                
                // Copy the bytes from the usb receive buffer into our structure so we
                // can use the information

                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Call MEGN540_Lab_Task Function
                Multiply_And_Send( data.v1, data.v2 );

                // /* MEGN540 -- LAB 2 */ 
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

                // /* MEGN540 -- LAB 2 */ 
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

                // /* MEGN540 -- LAB 2 */ 
                command_processed = true;
            }
            break;
        case '~':
            if( USB_Msg_Length() >= _Message_Length( '~' ) ) {
                // then process your reset by setting the task_restart flag defined in Lab1_Tasks.h

                // task cancel 
                // clear all buffers ? 
                // USB_Send_Byte( 0 ); 

                USB_Send_Byte( 0x6 ); 
                USB_Send_Byte( 0x66 ); 
                USB_Send_Byte( 0x0 );  
                USB_Send_Byte( 0x0 ); 
                USB_Send_Byte( 0x0 ); 
                USB_Send_Byte( 0x0 ); 
                USB_Send_Byte( 0x0 );

                Task_ReActivate( &task_restart );

                // /* MEGN540 -- LAB 2 */ 
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

                // float time_start = _time_since_last;
                // // something timing related to time how long the function takes 
                // // need to execute again and then time it 
                // Task_Message_Handling( _time_since_last ); // not sure what timing input 

                // float time_end = Timing_Get_Milli(); 
                // float time_total = time_end - time_start;
                
                // USB_Send_Msg( "cf", ' ', &time_total, sizeof( time_total ) );
                // // USB_Send_Msg( "cf", '-', &ret_val, sizeof( ret_val ) );   
                
                // process command 
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

                // uint8_t timing_operation = USB_Msg_Get();

                // uint8_t timing_interval = USB_Msg_Get(); 

                // do I want to make this a Task? It seems like it make a task with update time 
                // -1 or zero cancels rquest 


                // float time_start = _time_since_last;

                // // something timing related to time how long the function takes 
                // // need to execute again and then time it 
                // Task_Message_Handling( _time_since_last ); // not sure what timing input 

                // float time_end = Timing_Get_Milli(); 
                // float time_total = time_end - time_start;
                
                USB_Send_Msg( "cf", command_char, &time_interval, sizeof( time_interval ) );
                // USB_Send_Msg( "cf", '-', &ret_val, sizeof( ret_val ) );     

                // process command 
                command_processed = true; 
            }
            
            break; 
        default:
            // What to do if you dont recognize the command character
          
            USB_Send_Byte( 0x3 ); 
            USB_Send_Byte( 0x63 ); 
            USB_Send_Byte( 0x0 );  
            USB_Send_Byte( 0x3f );

            USB_Flush_Input_Buffer(); 


            // still have an issue with the buffer somehow 

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
    // if task was active for more than 
    if( ( Timing_Get_Milli() - _unused_ ) >= 250 ) 
    {
        // send wathdog message and rest 
        char command = '?';
        USB_Send_Msg( "cc", 'W', &command, sizeof( command ) );
        USB_Flush_Input_Buffer();
        Task_Cancel( &task_message_handling_watchdog ); 
    }
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
