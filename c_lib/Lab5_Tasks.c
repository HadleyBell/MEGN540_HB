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
        float left_meas = (skid_controller.wheel_diameter) * (Encoder_Rad_Left()/2);
        float left_vel = Controller_Update(&skid_controller.controller_right, left_meas, left_controller.update_period);
        left_vel = Saturate(left_vel, MotorPWM_Get_Max());
        MotorPWM_Set_Left(-left_vel);

        float right_meas = skid_controller.wheel_diameter * (Encoder_Rad_Right()/2);
        USB_Send_Msg("cf",'x', &right_meas, sizeof(right_meas));
        float right_vel = Controller_Update(&skid_controller.controller_right, right_meas, right_controller.update_period);
        USB_Send_Msg("cf",'v', &right_vel, sizeof(right_vel));
        right_vel = Saturate(right_vel, MotorPWM_Get_Max());
        MotorPWM_Set_Right(right_vel);

        MotorPWM_Enable( true );
        USB_Send_Msg("cf",'v', &right_vel, sizeof(right_vel));
        
    } else {
        // Disable motors
        struct __attribute__((__packed__)) {
        char let[7]; 
        float volt;} msg =  { 
            .let = {'B','A','T',' ','L','O','W'}, 
            .volt = Battery_Recent() }; 

        USB_Send_Msg("c7sf",'!', &msg, sizeof( msg )); 
        // Disable motors 
        MotorPWM_Enable( false );
    }
    // math?
}

// stop 
void PWM_Stop( float _time_since_last ) {
    
    Skid_Steer_Command_Velocity( Skid_Steer_Controller_t* p_skid_steer, 0, 0 );
    Skid_Steer_Command_Displacement( Skid_Steer_Controller_t* p_skid_steer, 0, 0 );

    // cancel the command control 
    Task_Cancel( &task_update_controllers_vel );

    // activate 
    Task_Activate( &task_pwm_stop, -1 );

    USB_Send_Byte( 0x8 ); 

}