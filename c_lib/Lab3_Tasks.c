#include "Lab3_Tasks.h"

void Send_Encoder_Count( float _time_since_last ) {

    struct __attribute__( ( __packed__ ) ) {
        float left;
        float right;
    } data;

    data.left = ( float ) Encoder_Counts_Left();
    data.right = ( float ) Encoder_Counts_Right();

    // return encoder coutns left and right 
    // int32_t left_count = Encoder_Counts_Right;  
    // int32_t right_count = _right_counts; 

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
    // // get battery voltage update with filter
    Battery_Voltage();
    // float current_voltage = Battery_Voltage();

    // // check if voltage is too low 
    // float min_voltage = 4.2; // equating to 1.05 V per cell nominal 1.2 V
    // // ALSO NEED TO CHECK IF SIWTCH IS ON 
    // if ( current_voltage < min_voltage ) { 
    //     struct __attribute__((__packed__)) {
    //         char let[7]; 
    //         float volt;} msg =  { 
    //             .let = {'B','A','T',' ','L','O','W'}, 
    //             .volt = Battery_Recent() }; 

    //     // Send Warning to Serial that batteries need to be charged 
    //     USB_Send_Msg("c7sf",'!', &msg, sizeof(msg));  
    // }
}