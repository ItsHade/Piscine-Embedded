
#include "main.h"

// sec example to convert value into decimal: (((0xf0 & value) >> 4) * 10) + (0x0f & value)

uint8_t get_time_value(uint8_t register_adress) {
    uint8_t value;

    i2c_start();
    // write bit is already shifted in address of doc ::::)
    // i2c_send_adr_as_sender(RTC_ADR);
    i2c_write(RTC_ADR);
    // send register address (only 4 bytes of right will be read)
    i2c_write(register_adress);
    i2c_stop();

    i2c_start();
    // enter read mode
    i2c_write(RTC_ADR + 1);
    value = i2c_read_nack();
    i2c_stop();
     /*
        Datasheet of PCF8563 -> page 6, describe all registers and how
        time values are stored. Most of it is in BCD, meaning data is stored on two units of
        four bit : one will hold unit, the other one the ten. 0001 0001 is read 11, not 17.
        This website holds a great example : https://www.digital-detective.net/binary-coded-decimal-timestamps/
    */
    return (value);
}


uint8_t    mode9(uint8_t setup) {
    uint8_t hour;
    uint8_t min;
    uint8_t value;

    value = get_time_value(HOUR_REGISTER);
    // accessing the right bits as described p.8
    hour = (((0b00110000 & value) >> 4) * 10) + (0x0f & value);
    value = get_time_value(MIN_REGISTER);
    // accessing the right bits as described p.8
    min = (((0b01110000 & value) >> 4) * 10) + (0x0f & value);
    
    uint8_t tab[12] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9, DISPLAY_DASH, DISPLAY_DOT};
    i2c_print_digits(I2C_EXPANDER_ADDR, tab[hour / 10], tab[hour % 10], tab[min / 10], tab[min % 10]);
    
    return (setup);
}

uint8_t     mode10(uint8_t setup) {
    uint8_t day;
    uint8_t month;
    uint8_t value;

    value = get_time_value(DAY_REGISTER);
    // accessing the right bits as described p.8
    day = (((0b00110000 & value) >> 4) * 10) + (0x0f & value);
    value = get_time_value(MONTH_REGISTER);
    // accessing the right bits as described p.9
    month = (((0b00010000 & value) >> 4) * 10) + (0x0f & value);

    uint8_t tab[12] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9, DISPLAY_DASH, DISPLAY_DOT};
    i2c_print_digits(I2C_EXPANDER_ADDR, tab[day / 10], tab[day % 10], tab[month / 10], tab[month % 10]);

    return (setup);
}

uint8_t     mode11(uint8_t setup) {
    uint8_t year;
    uint8_t century;
    uint8_t value;

    value = get_time_value(YEAR_REGISTER);
    // accessing the right bits as described p.9
    year = (((0xf0 & value) >> 4) * 10) + (0x0f & value);
    // century flag is the first bit of month register (p.9)
    value = get_time_value(MONTH_REGISTER);
    century = value & (1 << 7);
    if (century)
        century = 20;
    else
        century = 19;

    uint8_t tab[12] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9, DISPLAY_DASH, DISPLAY_DOT};
    i2c_print_digits(I2C_EXPANDER_ADDR, tab[century / 10], tab[century % 10], tab[year / 10], tab[year % 10]);

    return (setup);
}

void set_time_value(uint8_t *time_input, uint8_t start_address, uint8_t nb_bytes) {

    i2c_start();
    // write bit is already shifted in address of doc ::::)
    // i2c_send_adr_as_sender(RTC_ADR);
    i2c_write(RTC_ADR);
    // send register address (only 4 bytes of right will be read)
    i2c_write(start_address);
    /* i2c_write(byte); */
    i2c_multiwrite(time_input, nb_bytes);
    i2c_stop();
}

// check no value is too deresonably too big before setting the clock
// if time, cap days accordingly to months
uint8_t    format_is_valid(uint8_t *uart_input) {
    uint8_t max_values[TIME_ARRAY_SIZE] = {31, 12, 20, 99, 23, 59, 59};
    uint8_t min_values[TIME_ARRAY_SIZE] = {1, 1, 19, 0, 0, 0, 0};

    // input value should be contained between mx value and min value, otherwise its invalid
    for (uint8_t i = 0 ; i < TIME_ARRAY_SIZE; i++) {
        if ((uart_input[i] > max_values[i]) || (uart_input[i] < min_values[i]))
            return (0); 
    }
    return (1);
}

void debug_time_value(char *str, uint8_t value)
{
    putstr_uart("[DEBUG] ");
    putstr_uart(str);
    putnbr_uart(value);
    putstr_uart(" | 0x");
    puthex_uart(value);
    putstr_uart(NEW_LINE);
}

/* first page of man : built in word address reg is automatically
    incremented after each write or read
    Means -> from seconds to day, the memory is continuous, so we can just write to it
    */
void    rtc_set_time(uint8_t *uart_input) {
    uint8_t     time_input[TIME_ARRAY_SIZE - 1];

    // convert decimal to Binary Code Decimal and fill time_input, at the correct place before sending them to rtc registers
    // first three are easy, it is just the end of input reversed
    for (uint8_t i = 0 ; i < 3 ; i++)
        time_input[i] = ((uart_input[TIME_ARRAY_SIZE - i - 1] / 10) << 4) + (0x0f & (uart_input[TIME_ARRAY_SIZE - i - 1] % 10));
    // day at beginning of input
    time_input[3] = ((uart_input[0] / 10) << 4) + ((0x0f & uart_input[0] % 10));
    // month os second char of input
    time_input[4] = ((uart_input[1] / 10) << 4) + ((0x0f & uart_input[1] % 10));
    // if century is 20, flag a the first bit of month reg should be set
    if (uart_input[2] == 20)
        time_input[4] |= 0b10000000;
    // years from 0 to 99 are at 5 pos of input
    time_input[5] = ((uart_input[3] / 10) << 4) +((0x0f & uart_input[3] % 10));

    set_time_value(time_input, SEC_REGISTER, 4);
    set_time_value(time_input + 4, MONTH_REGISTER, 2);

    /* debug_time_value("index0: ", time_input[0]); */
    /* set_time_value(SEC_REGISTER, time_input[0]); */
    /* debug_time_value("index1: ", time_input[1]); */
    /* set_time_value(MIN_REGISTER, time_input[1]); */
    /* debug_time_value("index2: ", time_input[2]); */
    /* set_time_value(HOUR_REGISTER, time_input[2]); */
    /* debug_time_value("index3: ", time_input[3]); */
    /* set_time_value(DAY_REGISTER, time_input[3]); */
    /* debug_time_value("index4: ", time_input[4]); */
    /* set_time_value(MONTH_REGISTER, time_input[4]); */
    /* debug_time_value("index5: ", time_input[5]); */
    /* set_time_value(YEAR_REGISTER, time_input[5]); */
    /* putstr_uart("set_time_value() end\r\n"); */
}
