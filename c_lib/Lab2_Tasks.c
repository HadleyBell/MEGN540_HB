#include "Lab2_Tasks.h"

void Send_Loop_Time( float _time_since_last )
{
    // float current  = Timing_Get_Micro() * 0.1;
    // float ret_time = current - _time_since_last;
    // USB_Send_Msg( "cc", 't', &ret_time, sizeof( ret_time ) );
    // USB_Send_Msg( "ccf", 'T', &ret_time, sizeof( ret_time ) );
    static bool isidle     = true;
    static float runPeriod = 0;

    if( isidle ) {
        isidle                    = false;
        runPeriod                 = task_time_loop.run_period;
        task_time_loop.run_period = 0;
        return;
    }
    char command;
    if( runPeriod < 0 ) {
        command = 't';
    } else {
        command = 'T';
    }

    struct {
        uint8_t cmd;
        float time;
    } data = { .cmd = 0, .time = _time_since_last };

    USB_Send_Msg( "cBf", command, &data, sizeof( data ) );
    isidle                    = true;
    task_time_loop.run_period = runPeriod;
}

void Send_Time_Now( float _time_since_last )
{
    char command;
    if( task_send_time.run_period < 0 ) {
        command = 't';
    } else {
        command = 'T';
    }

    struct {
        uint8_t com;
        float time;
    } data = { .com = 0, .time = Timing_Get_Time_Sec() };
    // float data2 = Timing_Get_Time_Sec();

    USB_Send_Msg( "cBf", command, &data, sizeof( data ) );
}