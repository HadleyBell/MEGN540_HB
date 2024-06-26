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

/*
 * This file defines lab-spcific tasks for execution
 */

#ifndef LAB4_TASKS_H
#define LAB4_TASKS_H

// Min Motor Voltage for Running
#define BATTERY_MOTOR_MIN_VOLTAGE 4.75

// Include your lab-specific headers here
#include "Battery_Monitor.h"
#include "Encoder.h"
#include "MotorPWM.h"
#include "SerialIO.h"
#include "Task_Management.h"

// Put your lab-specific tasks here

Task_t task_pwm_set;
Task_t task_pwm_stop;
Task_t task_send_system_id;

// struct for left and right pwm
typedef struct __attribute__( ( __packed__ ) ) {
    uint16_t left;
    uint16_t right;
} PWMdata;

typedef struct __attribute__( ( __packed__ ) ) {
    uint16_t left;
    uint16_t right;
    float time;
} PWMdata_t;

// Put your lab-specific task functionality and data_structures (if necessary) here so it is accessable to both
// message handeling and the Lab main loops.
// e.g. void Send_Time_Now( float _time_since_last );

void Set_Left_Right_PWM( PWMdata pwm_data );
void Stop_and_Disable_PWM( float _time_since_last );
void Send_System_Id( float _time_since_last );

#endif  // ifndef LAB4_TASKS_H