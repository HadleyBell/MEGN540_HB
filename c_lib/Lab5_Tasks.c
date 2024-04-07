#include "Lab5_Tasks.h"

void Send_Encoders_position( float __unused)
{
    // Check battery voltage
    if( Battery_Recent() > BATTERY_MOTOR_MIN_VOLTAGE ) {
        // if battery voltage is ok then set
        MotorPWM_Enable( true );
    } else {
        // Disable motors
        MotorPWM_Enable( false );
    }

    // math?
}

void Send_Encoders_velocity( float __unused)
{
    // Check battery voltage
    if( Battery_Recent() > BATTERY_MOTOR_MIN_VOLTAGE ) {
        // if battery voltage is ok then set
        float left_meas = (3.14 * skid_controller.wheel_diameter) * Encoder_Rad_Left()/6.28;
        float left_vel = Controller_Update(&left_controller, left_meas, left_controller.update_period);
        left_vel = Saturate(left_vel, MotorPWM_Get_Max());
        MotorPWM_Set_Left(left_vel);

        float right_meas = (3.14 * skid_controller.wheel_diameter) * Encoder_Rad_Right()/6.28;
        float right_vel = Controller_Update(&right_controller, right_meas, right_controller.update_period);
        right_vel = Saturate(right_vel, MotorPWM_Get_Max());
        MotorPWM_Set_Right(right_vel);

        MotorPWM_Enable( true );
        
    } else {
        // Disable motors
        MotorPWM_Enable( false );
    }
    // math?
}