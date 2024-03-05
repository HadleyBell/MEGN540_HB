/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2021.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2021 Andrew Petruska at Colorado School of Mines

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

// put your includes here for all modules you need to use
#include "Battery_Monitor.h"   // for battery monitoring
#include "Encoder.h"           // for encoders
#include "Message_Handling.h"  // for translating USB messages to microcontroller tasks
#include "SerialIO.h"          // for USB communication
#include "Task_Management.h"   // for clean task management with functors
#include "Timing.h"            // for Time understanding

// Include Lab Sepcific Functionality
#include "Lab1_Tasks.h"
#include "Lab2_Tasks.h"
#include "Lab3_Tasks.h"

// put your global variables (only if absolutely required) here.
// Best to identify them as "static" to make them indentified as internal and start with a "_" to identify as internal.
// Try to initialize them if possible, so their values are never arbitrary.

// put your initialization function here
void Initialize_Modules( float _time_not_used_ )
{
    // Initialize (reinitialize) all global variables

    // reset USB input buffers
    USB_Flush_Input_Buffer();

    // Initialize all modules except USB (it can only be called once without messing things up)
    Initialize_Timing();
    Initialize_Encoders();
    Initialize_Battery_Monitor();
    Initialize_Battery_Filter();
    // GlobalInterruptEnable();

    // Setup task handling
    Initialize_Task( &task_restart, Initialize_Modules /*function pointer to call*/ );

    // Setup message handling to get processed at some desired rate.
    Initialize_Task( &task_message_handling, Task_Message_Handling );
    Task_Activate( &task_message_handling, 0.0 );

    Initialize_Task( &task_send_time, Send_Time_Now );

    Initialize_Task( &task_time_loop, Send_Loop_Time );

    Initialize_Task( &task_message_handling_watchdog, /*watchdog timout period*/ Task_Message_Handling_Watchdog );

    // Encoder tasks
    Initialize_Task( &task_send_encoders, Send_Encoders );

    // Battery tasks
    // will likely also just be the init
    // chance of needing a filter read/process/report task, so that time loop can do it
    Initialize_Task( &task_send_battery, Send_Battery );
    Initialize_Task( &task_battery_upkeep, Battery_Upkeep );
    Task_Activate( &task_battery_upkeep, 0.002 );
}
// put your task function definitions here

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main( void )
{
    Initialize_USB();
    Initialize_Modules( 0.0 );

    for( ;; ) {  // yet another way to do while (true)
        Task_USB_Upkeep();

        Task_Run_If_Ready( &task_message_handling );
        Task_Run_If_Ready( &task_restart );

        Task_Run_If_Ready( &task_send_time );
        Task_Run_If_Ready( &task_time_loop );
        Task_Run_If_Ready( &task_message_handling_watchdog );

        // Lab 3 tasks
        Task_Run_If_Ready( &task_send_encoders );
        Task_Run_If_Ready( &task_send_battery );
        // Task_Run_If_Ready( &task_battery_monitor );
        Task_Run_If_Ready( &task_battery_upkeep );
    }
}
