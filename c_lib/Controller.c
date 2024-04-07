#include "Controller.h"

/**
 * Function Saturate saturates a value to be within the range.
 */
inline float Saturate( float value, float ABS_MAX )
{
    return ( value > ABS_MAX ) ? ABS_MAX : ( value < -ABS_MAX ) ? -ABS_MAX : value;
}

/**
 * Function Initialize_Controller sets up the z-transform based controller for the system.
 */
void Initialize_Controller( Controller_t* p_cont, float kp, float* num, float* den, uint8_t order, float update_period )
{
    kp  = 0;
    num = 0;
    den = 0;
}

/**
 * Function Controller_Set_Target_Velocity sets the target velocity for the
 * controller.
 */
void Controller_Set_Target_Velocity( Controller_t* p_cont, float vel ) {}

/**
 * Function Controller_Set_Target_Position sets the target postion for the
 * controller, this also sets the target velocity to 0.
 */
void Controller_Set_Target_Position( Controller_t* p_cont, float vel ) {}

/**
 * Function Controller_Update takes in a new measurement and returns the
 * new control value.
 */
float Controller_Update( Controller_t* p_cont, float measurement, float dt ) {}

/**
 * Function Controller_Last returns the last control command
 */
float Controller_Last( Controller_t* p_cont ) {}

/**
 * Function Controller_SettTo sets the Filter's input and output lists
 * to match the measurement so it starts with zero error.
 */
void Controller_SetTo( Controller_t* p_cont, float measurement ) {}

/**
 * Function Controller_ShiftBy shifts the Filter's input and output lists
 * by the desired amount. This is helpful when dealing with wrapping.
 */
void Controller_ShiftBy( Controller_t* p_cont, float measurement ) {}
