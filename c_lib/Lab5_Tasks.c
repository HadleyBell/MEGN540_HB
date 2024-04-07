#include "Lab5_Tasks.h"

void Send_Encoders_position( distanceData data )
{
    // Check battery voltage
    if( Battery_Recent() > BATTERY_MOTOR_MIN_VOLTAGE ) {
        // if battery voltage is ok then set
        Controller_Set_Target_Velocity( data.linear );
        Controller_Set_Target_Position( data.angular );
    } else {
        // Disable motors
        MotorPWM_Enable( false );
    }

    // math?
}

void Send_Encoders_velocity( velocityData data )
{
    // Check battery voltage
    if( Battery_Recent() > BATTERY_MOTOR_MIN_VOLTAGE ) {
        // if battery voltage is ok then set
        Controller_Set_Target_Velocity( data.linear );
        Controller_Set_Target_Position( data.angular );
    } else {
        // Disable motors
        MotorPWM_Enable( false );
    }
    // math?
}