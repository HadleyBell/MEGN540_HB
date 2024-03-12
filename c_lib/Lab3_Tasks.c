#include "Lab3_Tasks.h"

void Send_Encoders( float _time_since_last )
{
    char action = ( task_send_encoders.run_period < 0 ) ? 'e' : 'E';

    struct {
        float left_e;
        float right_e;
    } data = { .left_e = (float)Encoder_Counts_Left(), .right_e = (float)Encoder_Counts_Right() };

    USB_Send_Msg( "cff", action, &data, sizeof( data ) );
}

void Send_Battery( float _time_since_last )
{
    char action = ( task_send_battery.run_period < 0 ) ? 'b' : 'B';

    float voltage = Filter_Last_Output( &battery_lowpass );

    if( voltage < 3.2 ) {
        Battery_Low( voltage );
        Task_USB_Upkeep();
    }

    USB_Send_Msg( "cf", action, &voltage, sizeof( voltage ) );
}

void Battery_Upkeep( float _time_since_last )
{
    // needs to be activated with 2ms period
    // read raw voltage
    float new_voltage = Battery_Voltage();

    // pass new voltage into filter inputs
    Filter_Value( &battery_lowpass, new_voltage );
}

void Battery_Low( float bat_volt )
{
    struct __attribute__( ( __packed__ ) ) {
        char let[7];
        float volt;
    } msg = { .let = { 'B', 'A', 'T', ' ', 'L', 'O', 'W' }, .volt = bat_volt };
    // Send Warning to Serial that batteries need to be charged
    USB_Send_Msg( "c7sf", '!', &msg, sizeof( msg ) );
}

void Initialize_Battery_Filter()
{
    // sets up filter
    float num[] = { 0.0134, 0.0267, 0.0134 };   // b
    float den[] = { 1.0000, -1.6475, 0.7009 };  // a
    int order   = 2;

    Filter_Init( &battery_lowpass, num, den, order );
    // Filter_ShiftBy( &battery_lowpass, Battery_Voltage() );

    // fill initial slot in filter input
    // Filter_Value( &battery_lowpass, Battery_Voltage())
}