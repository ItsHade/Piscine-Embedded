#include "main.h"

/*  
    all doc about thermosensor is in 
    https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf
*/

/* This function will follow steps described in 5.4.3 and 5.4.4.
    They are summarised into the small table at the end */
uint8_t    is_sensor_data_ready(void) {
    uint8_t byte = 0;

    // Ask for data
    i2c_start();
    // write address as sender
    i2c_write(AHT20_ADR << 1);
    // write sequence to ask for data
    i2c_multiwrite(AHT20_DATA, 3);
    i2c_stop();

    i2c_start();
    // write address as receiver
    i2c_write(AHT20_ADR << 1 | 1);
    // Wait for data ready
    byte = i2c_read();
    i2c_stop();
    // data is not ready! wait for next interrupt flag
    if (!((byte & 1) == 0))
        return (0);
    return (1);
}

void     read_sensor_data(uint8_t* data_byte) {
    i2c_start();
    // write address as receiver
    i2c_write(AHT20_ADR << 1 | 1);
    // Receive data
    i2c_multiread(data_byte, 6);
    // get the last one, but this time send no ack in exchange
    data_byte[6] = i2c_read_nack();
    i2c_stop();
    // if we got time, do a check CRC to make sure the value is read
    // properly, is not just keep previous value
}

void    calibrate_Aht20(void) {
    uint8_t byte;
    uint8_t clear_sensor_vals[7];

    // Part 5.1 of doc : wait at top 20ms for sensor to be in idle state
    // also states to wait 40 ms after device power on before sending data so we'll wait 40
    _delay_ms(40);
    // maybe we can remove it

    i2c_start();
    // initiate transaction as sender
    i2c_write(AHT20_ADR << 1);
    // ask for aht20 status
    i2c_write(AHT20_STATUS);

    //Send a double start condition to reset transaction on controller receive
    i2c_start();
    // enter connection in receive mode from aht20
    i2c_write(AHT20_ADR << 1 | 1);
    // set the status 
    byte = i2c_read();
    /*
        5.4.1 -> chech bit [3] to make sure calibration is done
        if not, enter calibration command.
        The arguments for the command are then specified with 0x08 and 0x00, given by same datasheet
        Then a smol delay of 10 is required
    */
    if (byte & (1 << 3)) {
        //Send a double start condition to reset transaction on controller send
        i2c_start();
        i2c_write(AHT20_ADR << 1);
        i2c_multiwrite(AHT20_CALIBR, 3);
        _delay_ms(10);
    }

    read_sensor_data(clear_sensor_vals);
    // Do 2 readings at startup in order to get corrects values
    _delay_ms(2000);
    read_sensor_data(clear_sensor_vals);
}

uint8_t*    convert_values(uint8_t *data, uint8_t mode, uint8_t *result) {
    float       computed_value;
    uint32_t    raw_temp;
    uint32_t    raw_humidity;

    // To convert values, we will follow the documentation about rteading and converting values
    // see 5.4 to read what offset, and 5.5 to convert offset afterwards
    // Everything should be stored into a uint32, as values are on 20 bits
    raw_humidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | ((uint32_t)data[3] >> 4);
    raw_temp = ((uint32_t)(data[3] & 0x0f) << 16) | ((uint32_t)data[4] << 8) |(uint32_t)data[5];

    // follow 5.5 formulas to translate from raw data to a float
    if (mode == 6)
        computed_value = (((float)raw_temp / 1048576.0) * 200.0 - 50.0);
    // This one is same as previous, w/ a little salt for Fareneith conversion
    else if (mode == 7)
        computed_value = ((((float)raw_temp / 1048576.0) * 200.0 - 50.0) * 1.8) + 32;
    else
        computed_value = (((float)raw_humidity / 1048576.0) * 100.0);

    // Convert value from float to 2 uint, one for part before coma, one for part after
    // Get the full part of value, then substract it , then multiplies by 100 gets us 2 decimal precision
    result[0] = (uint8_t)computed_value;
    result[1] = (uint8_t)((computed_value - result[0]) * 100);
    
    return (result);
}

void display_sensor(uint8_t* values) {

    uint8_t tab[12] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9, DISPLAY_DASH, DISPLAY_DOT};
    i2c_print_digits(I2C_EXPANDER_ADDR, tab[values[0] / 10],\
                                        (tab[values[0] % 10] | tab[11]),\
                                        tab[values[1] / 10],\
                                        tab[values[1] % 10]);
}

// Mode 6 = displays temperature in Celsius
uint8_t mode6(uint8_t setup) {
    uint8_t data_byte[7];
    static uint8_t celsius_result[2] = {0, 0};
    static uint8_t secs = 2;

    display_sensor(celsius_result);
    if (setup)
        secs = 2;
    if (TIFR1 & (1 << OCF1A)) {
            ++secs;
            TIFR1 |= (1 << OCF1A);
        }
    // if delay of 2 sec is not out, display previous data & wait
    if (secs < 2) {
        return (0);
    }
    if (TIFR0 & (1 << OCF0A)) {
        if (is_sensor_data_ready()) {
            read_sensor_data(data_byte);
            convert_values(data_byte, 6, celsius_result);
            secs = 0;
        }
        // display_sensor(celsius_result);
        // clear flag to resend 80 ms
        TIFR0 |= (1 << OCF0A);        
    }
    return (setup);
}

// Mode 7 = displays temperature in Farenheit
uint8_t mode7(uint8_t setup) {
    uint8_t data_byte[7];
    static uint8_t fareneith_result[2] = {0, 0};
    static uint8_t secs = 0;

    display_sensor(fareneith_result);
    if (setup)
        secs = 2;
    if (TIFR1 & (1 << OCF1A)) {
            ++secs;
            TIFR1 |= (1 << OCF1A);
        }
    // if delay of 2 sec is not out, display previous data & wait
    if (secs < 2) {
        return (0);
    }
    if (TIFR0 & (1 << OCF0A)) {
        if (is_sensor_data_ready()) {
            read_sensor_data(data_byte);
            convert_values(data_byte, 7, fareneith_result);
            secs = 0;
        }
        // display_sensor(celsius_result);
        // clear flag to resend 80 ms
        TIFR0 |= (1 << OCF0A);        
    }
    return (setup);
}

// Mode 8 = displays humidity in percentage
uint8_t mode8(uint8_t setup) {
    uint8_t data_byte[7];
    static uint8_t humidity_result[2] = {0, 0};
    static uint8_t secs = 0;


    display_sensor(humidity_result);
    if (setup)
        secs = 2;
    if (TIFR1 & (1 << OCF1A)) {
            ++secs;
            TIFR1 |= (1 << OCF1A);
        }
    // if delay of 2 sec is not out, display previous data & wait
    if (secs < 2) {
        return (0);
    }
    if (TIFR0 & (1 << OCF0A)) {

    putnbr_uart(humidity_result[0]);
    putstr_uart(".");
    putnbr_uart(humidity_result[1]);
    putstr_uart("\r\n");
    putstr_uart("\r\n");
        if (is_sensor_data_ready()) {
            read_sensor_data(data_byte);
            convert_values(data_byte, 8, humidity_result);
            secs = 0;
        }
        // display_sensor(celsius_result);
        // clear flag to resend 80 ms
        TIFR0 |= (1 << OCF0A);        
    }
    return (setup);
}


uint8_t    setup_timer1(uint8_t setup)
{
    init_timer1(1);
    return (setup);
}

uint8_t    setup_timers(uint8_t setup)
{
    init_timer0();
    init_timer1(1);
    return (setup);
}
