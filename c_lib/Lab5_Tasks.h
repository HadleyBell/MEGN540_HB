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

#ifndef LAB5_TASKS_H
#define LAB5_TASKS_H

// Min Motor Voltage for Running
#define BATTERY_MOTOR_MIN_VOLTAGE 4.75

// Include your lab-specific headers here
#include "Battery_Monitor.h"
#include "Encoder.h"
#include "MotorPWM.h"
#include "SerialIO.h"
#include "Task_Management.h"
#include "Skid_Steer_Controller.h"
#include "Controller.h"

// struct for left and right pwm
typedef struct __attribute__( ( __packed__ ) ) {
    float linear;
    float angular;  // 0 if you dont want the car to turn
} distanceData;

typedef struct __attribute__( ( __packed__ ) ) {
    float linear;
    float angular;
} velocityData;

// Put your lab-specific tasks here
Task_t task_update_controllers;
Controller_t left_controller;
Controller_t right_controller;
Skid_Steer_Controller_t skid_controller;


void update_controller();

#endif  // ifndef LAB5_TASKS_H