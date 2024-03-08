#include "Lab3_Tasks.h"


// equating to 1.1 V per cell nominal 1.2 V
#define BATTERY_MIN_VOLTAGE 4.75
// lowest voltage qick dropoff after switch turned off
#define BATTERY_CONNECTED_VOLTAGE 2.5

void Send_Encoder_Count( float _time_since_last ) {

    struct __attribute__( ( __packed__ ) ) {
        float left;
        float right;
    } data;

    // sending counts
    data.left = ( float ) Encoder_Counts_Left();
    data.right = ( float ) Encoder_Counts_Right();
    // // sending rads 
    // data.left = ( float ) Encoder_Rad_Left();
    // data.right = ( float ) Encoder_Rad_Right();

    // determine if command was 'e' or 'E' 
    char t_type; 
    if( task_send_encoder_count.run_period > 0 ) {
        t_type = 'E'; 
    } else {
        t_type = 'e';
    }

    // send left and right envoder over usb 
    USB_Send_Msg( "cff", t_type, &data, sizeof( data ) );
}

void Send_Battery_Voltage( float _time_since_last ) {
    // return most recent reading 
    float bat_reading = Battery_Recent(); 

    // determine if command was 'b' or 'B' 
    char t_type; 
    if( task_send_battery_voltage.run_period > 0 ) {
        t_type = 'B'; 
    } else {
        t_type = 'b';
    }

    // send battery reading voltage over usb 
    USB_Send_Msg( "cf", t_type, &bat_reading, sizeof( bat_reading ) ); 
}


void Update_Battery_Voltage( float _time_since_last ) {
    // get battery voltage update with filter
    Battery_Voltage();
}

void Low_Battery_Voltage( float _time_since_last ) {
    // low battery structure 
    struct __attribute__((__packed__)) {
        char let[7]; 
        float volt;} msg =  { 
            .let = {'B','A','T',' ','L','O','W'}, 
            .volt = Battery_Recent() }; 

    // check if voltage is too low but battery is plugged in
    if ( ( msg.volt < BATTERY_MIN_VOLTAGE ) && ( msg.volt > BATTERY_CONNECTED_VOLTAGE ) ) { 
        // Send low battery message 
        USB_Send_Msg("c7sf",'!', &msg, sizeof( msg )); 
        // Disable motors 
        MotorPWM_Enable( false ); 
        // Clear motor task 
        
    } 
}