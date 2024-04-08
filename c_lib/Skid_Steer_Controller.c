#include "Skid_Steer_Controller.h"

void Initialize_Skid_Steer( Skid_Steer_Controller_t* p_skid_steer, float* z_transform_numerator, float* z_transform_denominator, uint8_t z_transform_order,
                            float descritization_period, float error_to_control_gain, float max_abs_control, float wheel_base_width, float wheel_diameter,
                            float ( *measurement_left_fcn_ptr )( void ), float ( *measurement_right_fcn_ptr )( void ), void ( *control_left_fcn_ptr )( float ),
                            void ( *control_right_fcn_ptr )( float ) )
{
    // initalize controllers left and right
    float kp = 1.4775;
    p_skid_steer->wheel_diameter = wheel_diameter;
    USB_Send_Message("cf", 'w', &p_skid_steer->wheel_diameter, sizeof(p_skid_steer->wheel_diameter));
    p_skid_steer->wheel_base_width = wheel_base_width;
    Initialize_Controller( &p_skid_steer->controller_left, kp, z_transform_numerator, z_transform_denominator, z_transform_order, descritization_period );
    Initialize_Controller( &p_skid_steer->controller_right, kp, z_transform_numerator, z_transform_denominator, z_transform_order, descritization_period );
    // p_skid_steer->measurement_left_fcn_ptr = Encoder_Counts_Left;
    // p_skid_steer->measurement_right_fcn_ptr = Encoder_Counts_Right;
}

/**
 * @brief Skid_Steer_Command_Displacement sets a new target diplacment for the robot to execute. This is a relative displacment to the current position, not an
 * absolute target.
 *
 * @param linear The arc-length to travel (m)
 * @param angular The angle to rotate (rad)
 */
void Skid_Steer_Command_Displacement( Skid_Steer_Controller_t* p_skid_steer, float linear, float angular )
{
    // get current displacement
    int32_t left_counts  = p_skid_steer->measurement_left_fcn_ptr();
    int32_t right_counts = p_skid_steer->measurement_right_fcn_ptr();

    // convert to floats
    float left_counts_f  = (float)left_counts;
    float right_counts_f = (float)right_counts;

    // get left and right displacement from skid steer dynamics
    float left_displacement  = Left_Displacement( p_skid_steer, linear, angular );
    float right_displacement = Right_Displacement( p_skid_steer, linear, angular );

    // set filter to current pwm
    // problem is that it sets input (measurements) and output
    // Controller_SetTo( p_skid_steer.controller_left, left_displacement );

    // need to increment from previous displacement
    Controller_Set_Target_Position( &p_skid_steer->controller_right, right_displacement + right_counts_f );
    Controller_Set_Target_Position( &p_skid_steer->controller_left, left_displacement + left_counts_f );
}

/**
 * @brief Skid_Steer_Command_Velocity sets a target velocity for the skid-steer system to execute
 *
 * @param linear The tangential velocity to move at (m/s)
 * @param angular The angular rate to spin at (rad/s)
 */
void Skid_Steer_Command_Velocity( Skid_Steer_Controller_t* p_skid_steer, float linear, float angular )
{

    // get encoder counts for left and right
    int32_t left_count  = p_skid_steer->measurement_left_fcn_ptr();
    int32_t right_count = p_skid_steer->measurement_right_fcn_ptr();

    // convert to floats
    float left_count_f  = (float)left_count;
    float right_count_f = (float)right_count;

    float left_control_new  = Controller_Update( &p_skid_steer->controller_left, left_count_f, p_skid_steer->controller_left.update_period );
    float right_control_new = Controller_Update( &p_skid_steer->controller_right, right_count_f, p_skid_steer->controller_right.update_period );

    // set motor
    MotorPWM_Set_Left( (int16_t)left_control_new );
    MotorPWM_Set_Right( (int16_t)right_control_new );
}

/**
 * @brief Skid_Steer_Control_Update executes a control update by comparing current measurments desired values and implements new control setpoints.
 *
 */
void Skid_Steer_Control_Update( Skid_Steer_Controller_t* p_skid_steer, float ellapsed_time ) {}

float Left_Displacement( Skid_Steer_Controller_t* p_skid_steer, float linear, float angular )
{
    float left_d = linear + angular * p_skid_steer->wheel_base_width / 2;
    return left_d;
}
float Right_Displacement( Skid_Steer_Controller_t* p_skid_steer, float linear, float angular )
{
    float right_d = linear - angular * p_skid_steer->wheel_base_width / 2;
    return right_d;
}