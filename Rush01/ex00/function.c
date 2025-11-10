#include "main.h"

// void i2c_set_pin(uint8_t addr, uint8_t command, uint8_t pinsValue)
// {
//     i2c_start();
//     i2c_write((addr << 1 ) | I2C_WRITE); 
//     i2c_write(command);
//     i2c_write(pinsValue); // re write the whole byte -> probably need to |= and change my defines and enum 
//     i2c_stop();
// }

void	adc_init(void)
{
    // Configure ADC for 10-bit right adjusted result, AVCC as reference
    ADMUX = 0; //clear ADMUX register
    ADMUX |= (1 << REFS0); // AVCC as reference
    ADMUX &= ~(1 << ADLAR); // right adjust for 10-bit result (ADCL + ADCH)
    ADCSRA |= (1<<ADEN); // turn on ADC
    ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)); // prescaler 128 for 16MHz
}

uint16_t	adc_read(uint8_t ch)
{
    adc_init();
    // select ADC channel ch must be 0-7
    ch &= 0b00000111; // limit to 0-7
    ADMUX = (ADMUX & 0xF8) | ch; // set channel, preserve REFS0 and ADLAR

    ADCSRA |= (1<<ADSC); // start single conversion
    while(!(ADCSRA & (1<<ADIF))); // wait for conversion to complete
    ADCSRA |= (1<<ADIF); // clear ADIF by writing one to it

    // read ADCL first then ADCH (required) for 10-bit value
    uint16_t value = ADCL;
    value |= ((uint16_t)ADCH) << 8;
    return value & 0x03FF; // mask to 10 bits
}

// static inline void select_channel(uint8_t channel)
// {
// 	PORTC = (PORTC & 0xF0) | (channel & 0x0F);
// }

uint16_t read_RV1(void)
{
    static int w = 0;
    //select_channel(0); // Select RV1
    while (w++ < 10000)
    {}
    return adc_read(0);
}

uint16_t read_LDR(void)
{
    static int w = 0;
    
    while (w++ < 30000)
    {
        static int j = 0;
        while (j++ < 30000)
        {}
    }
     // Allow settling time for ADC1 (PC1)
    return adc_read(1); // Read from ADC1 channel directly
}

uint16_t read_NTC(void)
{
    static int w = 0;
    while (w++ < 30000)
    {
        static int j = 0;
        while (j++ < 30000)
        {}
    }
    return adc_read(2); // Read from ADC2 channel directly
}


uint16_t read_Temp(void)
{
    ADMUX = (3 << REFS0) | (8 << MUX0); // 1.1V REF, channel#8 is temperature
    ADCSRA |= (1 << ADEN) | (6 << ADPS0);       // enable the ADC div64
    
    //wait until voltages are stable
    volatile int w = 0;
    while (w++ < 6000)
    {}

    ADCSRA |= (1 << ADSC);      // Start the ADC
    while (ADCSRA & (1 << ADSC));       // Detect end-of-conversion
    return (ADC);
}


uint8_t display_RV1(uint8_t setup)
{
    uint16_t adc_value = read_RV1();
    //adc_value is 0..1023 (10-bit). Display full decimal value on UART and 4-digit 7-seg.
    uint16_t display_value = adc_value; // 0 - 1023
    
    // all bellow could be replaced by i2c_set_digits()
    // same for following functions


    uint8_t tab[12] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9, DISPLAY_DASH, DISPLAY_DOT};
    i2c_print_digits(I2C_EXPANDER_ADDR, tab[(display_value / 1000) % 10], tab[(display_value / 100) % 10], tab[(display_value / 10) % 10], tab[display_value % 10]);
    
    return (setup);
}

uint8_t    display_LDR(uint8_t setup)
{
    uint16_t adc_value = read_LDR();

    //round the value by averaging multiple samples
    static uint16_t last_value = 0;
    // Apply exponential moving average: new = 0.7*current + 0.3*last
    adc_value = (uint16_t)((adc_value * 7 + last_value * 3) / 10);
    last_value = adc_value;

    uint8_t tab[12] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9, DISPLAY_DASH, DISPLAY_DOT};
    i2c_print_digits(I2C_EXPANDER_ADDR, tab[(last_value / 1000) % 10], tab[(last_value / 100) % 10], tab[(last_value / 10) % 10], tab[last_value % 10]);
    return (setup);
}
// Basic main that makes the selected LED blink
// Address programation need to be correct

uint8_t    display_NTC(uint8_t setup)
{
    uint16_t adc_value = read_NTC();
    //adc_value is 0..1023 (10-bit). Display full decimal value on UART and 4-digit 7-seg.

    static uint16_t last_value = 0;
    // Apply exponential moving average: new = 0.7*current + 0.3*last
    adc_value = (uint16_t)((adc_value * 7 + last_value * 3) / 10);
    last_value = adc_value;

    uint8_t tab[12] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9, DISPLAY_DASH, DISPLAY_DOT};
    i2c_print_digits(I2C_EXPANDER_ADDR, tab[(last_value / 1000) % 10], tab[(last_value / 100) % 10], tab[(last_value / 10) % 10], tab[last_value % 10]);
    return (setup);
}

uint8_t    display_temp(uint8_t setup)
{
    static uint16_t refresh_count = 0;
    static uint16_t temp_display = 0; // last temperature value to show
    if (setup)
    {
        refresh_count = 0;
        //temp_display = 0;
    }
    // double at = (temp_display < 0) ? -temp_display : temp_display;

    putnbr_uart(temp_display);
    putstr_uart(NEW_LINE);
    // uint16_t temp_int = (int16_t)at; // integer part
    // uint8_t temp_frac = (uint8_t)((at - temp_int) * 100); // fractional part (2 digits, rounded)
    uint8_t tab[12] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9, DISPLAY_DASH, DISPLAY_DOT};
    i2c_print_digits(I2C_EXPANDER_ADDR, tab[(temp_display / 1000) % 10], tab[(temp_display / 100) % 10], tab[(temp_display / 10) % 10], tab[temp_display % 10]);


    // Timer1 configuration for CTC 1s (no ISR)
    // Configure once and then do a non-blocking poll of the compare flag
    // F_CPU = 16 MHz, prescaler = 1024 -> tick = 15625 counts per second
    // OCR1A = 15624 gives approx 1 second
    // static uint8_t timer1_init = 0;
    // if (!timer1_init) {
    //     TCCR1A = 0;                 // normal port operation
    //     TCCR1B = (1 << WGM12);     // CTC mode (WGM12=1)
    //     OCR1A = 15624;             // compare value for ~1s
    //     TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler = 1024
    //     timer1_init = 1;
    // }

    // // Non-blocking: if one second has elapsed, the OCF1A flag will be set
    // if (TIFR1 & (1 << OCF1A)) {
    //     // clear flag by writing 1
    //     TIFR1 |= (1 << OCF1A);
    //     // 1 second elapsed -> read temperature and update stored value
    //     double temp = read_Temp();
    //     double t = (temp >= 0 ? (temp + 0.5) : (temp - 0.5)); // round
    //     temp_display = t;
    // }


    // after one full refresh, increment counter; update/print once per ~1000ms
    refresh_count++;
    // one full refresh takes ~12ms; ~83 refreshes => ~996ms
    if (refresh_count >= 100) {
        refresh_count = 0;
        // read temperature ONCE and update displayed value

        temp_display = read_Temp();
        // double t = (temp >= 0 ? (temp + 0.5) : (temp - 0.5)); // round
        // temp_display = t;
    }

    return (setup);
}
