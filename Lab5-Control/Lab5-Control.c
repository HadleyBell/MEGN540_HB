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

#include "Battery_Monitor.h"   // for battery monitor
#include "Encoder.h"           // for motor rotational counting
#include "Message_Handling.h"  // for translating USB messages to microcontroller tasks
#include "MotorPWM.h"          // for drive motors
#include "SerialIO.h"          // for USB communication
#include "Task_Management.h"   // for clean task management with functors
#include "Timing.h"            // for Time understanding
#include "Skid_Steer_Controller.h"
#include "Controller.h"

// Include Lab Sepcific Functionality
#include "Lab1_Tasks.h"
#include "Lab2_Tasks.h"
#include "Lab3_Tasks.h"
#include "Lab4_Tasks.h"
#include "Lab5_Tasks.h"

// put your global variables (only if absolutely required) here.task_send_time
// Best to identify them as "static" to make them indentified as internal and start with a "_" to identify as internal.
// Try to initialize them if possible, so their values are never arbitrary.

/**
 * Function Initialize_Modules sets up all hardware and persistant memory necessary
 * for the main loop to funciton properly. It is the first thing main should call and is
 * a convenient way or resetting the system if that is requested.
 *
 * It takes a float as a parameter that is not used to allow it to integrate nicely with the Task_Management Code (which should pass it the seconds since last
 * ran)
 *
 */
void Initialize_Modules( float _time_not_used_ )
{
    // Initialize (reinitialize) all global variables

    // reset USB input buffers
    USB_Flush_Input_Buffer(); 

    // Initialize all modules except USB (it can only be called once without messing things up)
    Initialize_Timing();

    // Initialize encoders 
    Initialize_Encoders(); 

    // Initalize battery 
    Initialize_Battery_Monitor(); 

    // Initalize motor 
    Initialize_MotorPWM( 500 ); 

    // Setup task handling
    Initialize_Task( &task_restart, Initialize_Modules /*function pointer to call*/ );

    // Setup message handling to get processed at some desired rate.
    Initialize_Task( &task_message_handling, Task_Message_Handling );
    // Initialize_Task( &task_message_handling_watchdog, /*watchdog timout period*/,  Task_Message_Handling_Watchdog );
    Initialize_Task( &task_message_handling_watchdog, Task_Message_Handling_Watchdog );

    Task_Activate( &task_message_handling, 0 ); 

    // timing tasks 
    // Send_Loop_Time(1.001);
    Initialize_Task( &task_time_loop, Send_Loop_Time );
    Initialize_Task( &task_send_time, Send_Time_Now );

    // Encoder Tasks 
    Initialize_Task( &task_send_encoder_count, Send_Encoder_Count );

    // Battery Update
    Initialize_Task( &task_update_battery_voltage, Update_Battery_Voltage ); 
    Task_Activate( &task_update_battery_voltage, 0.002 ); // Activate to update every 2 ms
    // Battery Low 
    Initialize_Task( &task_low_battery_voltage, Low_Battery_Voltage );
    Task_Activate( &task_low_battery_voltage, 1000.0 ); // Activate to run every 1 sec
    // Battery Send 
    Initialize_Task( &task_send_battery_voltage, Send_Battery_Voltage );

    // Motor Stop and Dissable
    Initialize_Task( &task_pwm_stop, Stop_and_Disable_PWM ); 
    // Initialize_Task( &task_pwm_set, Set_Left_Right_PWM );

    // System Identification
    Initialize_Task( &task_send_system_id, Send_System_Id );

    float a[2] = {1.00, -0.9975};
    float b[2] = {2.43, 2.41};

    skid_controller.controller_left = left_controller;
    skid_controller.controller_right = right_controller;

    Initialize_Skid_Steer(&skid_controller, b, a, 1, 0.01, 1.0, 1.0, 0.066, 0.039, NULL, NULL, NULL, NULL);
    Initialize_Task(&task_update_controllers_vel, Send_Encoders_velocity);
}

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main( void )
{
    Initialize_USB();
    Initialize_Modules( 0.0 );

    for( ;; ) {  // yet another way to do while (true)
        Time_t loop_start_time = Timing_Get_Time();

        // update battery every 2 ms but only return resut if error low bat

        Task_USB_Upkeep();

        // usb message inputs 
        Task_Run_If_Ready( &task_message_handling );

        // timing 
        Task_Run_If_Ready( &task_send_time );
        Task_Run_If_Ready( &task_time_loop );

        // encoder 
        Task_Run_If_Ready( &task_send_encoder_count );
        // battery 
        Task_Run_If_Ready( &task_send_battery_voltage );
        Task_Run_If_Ready( &task_update_battery_voltage ); 
        Task_Run_If_Ready( &task_low_battery_voltage ); 

        // usb watchdog 
        Task_Run_If_Ready( &task_message_handling_watchdog );
        Timing_Set_Loop_Time( loop_start_time );

        // motor 
        Task_Run_If_Ready( &task_pwm_stop ); 

        // System ID 
        Task_Run_If_Ready( &task_send_system_id ); 

        Task_Run_If_Ready( &task_update_controllers_vel);

        // re initalize
        Task_Run_If_Ready( &task_restart );
    }
}
