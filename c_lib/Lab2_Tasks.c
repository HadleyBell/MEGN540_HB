#include "Lab2_Tasks.h"

void Send_Loop_Time( float _time_since_last ){

    float ret_time = _time_since_last + Timing_Seconds_Since();
    USB_Send_Msg( "cc", 't');

}
void Send_Time_Now( float _time_since_last ){
    
    float ret_time = _time_since_last + Timing_Get_Time_Sec() ;
    USB_Send_Msg( "ccf", 'T');
}