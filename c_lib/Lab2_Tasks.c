#include "Lab2_Tasks.h"

void Send_Loop_Time( float _time_since_last ) 
{
    // return value structure containing command and float 
    struct __attribute__( ( __packed__ ) ) 
    {
        unsigned short H; 
        float f; 
    } ret_val;

    uint32_t loop_time_micro = Timing_Get_Loop_Time(); 

    ret_val.H = 0x01; 
    // compute time from last run 
    ret_val.f = (float)loop_time_micro; 

    // determine if command was 'T' or 't' 
    char t_type; 
    if( task_time_loop.run_period > 0 ) {
        t_type = 'T'; 
    } else {
        t_type = 't';
    }

    // return the time to be sent 
    USB_Send_Msg( "cHf",t_type, &ret_val, sizeof( ret_val ) );    
};


void Send_Time_Now( float _time_since_last ) 
{
    // return value structure containing command and float 
    struct __attribute__( ( __packed__ ) ) 
    {
        unsigned short H; 
        float f; 
    } ret_val;

    ret_val.H = 0x00; 
    // ret_val.f = task_send_time.run_period; 
    ret_val.f = Timing_Get_Time_Sec(); 

    // determine if command was 'T' or 't' based on run period 
    char t_type; 
    if( task_send_time.run_period > 0 ) {
        t_type = 'T'; 
    } else {
        t_type = 't';
    }

    // return the time to be sent 
    USB_Send_Msg( "cHf",t_type, &ret_val, sizeof( ret_val ) );    
};
