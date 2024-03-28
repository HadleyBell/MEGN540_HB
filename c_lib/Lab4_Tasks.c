#include "Lab4_Tasks.h"

// need a way to send pwm for a given duration

// Not sure if this is needed will see through
void Set_Left_Right_PWM( PWMdata pwm_data )
{
    // Check battery voltage
    if( Battery_Recent() > BATTERY_MOTOR_MIN_VOLTAGE ) {
        // if battery voltage is ok then set
        MotorPWM_Set_Left( pwm_data.left );
        MotorPWM_Set_Right( pwm_data.right );
    } else {
        // Disable motors
        MotorPWM_Enable( false );
    }
}

void Stop_and_Disable_PWM( float _time_since_last )
{
    // Disable motors
    MotorPWM_Enable( false );
    // Stop PWM
    MotorPWM_Set_Left( 0 );
    MotorPWM_Set_Right( 0 );
}

void Send_System_Id( float _time_since_last )
{
    char operator;
    // struct for left and right pwm
    struct __attribute__( ( __packed__ ) ) {
        float time;
        int16_t pwm_left;
        int16_t pwm_right;
        int16_t encoder_left;
        int16_t encoder_right;
    } data;

    // Place data into struct
    data.time          = Timing_Get_Time_Sec();
    data.pwm_left      = MotorPWM_Get_Left();
    data.pwm_right     = MotorPWM_Get_Right();
    data.encoder_left  = (int16_t)Encoder_Counts_Left();
    data.encoder_right = (int16_t)Encoder_Counts_Right();
    if( task_send_system_id.run_period > 0 ) {
        operator= 'Q';
    } else {
        operator= 'q';
    }

    // Send message
    USB_Send_Msg( "cf4h", operator, & data, sizeof( data ) );
}