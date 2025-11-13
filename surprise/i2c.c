#include "main.h"


void i2c_init(void)
{
    TWSR = 0; // Prescaler = 1
              // 22.9.1 TWBR - TWI Bit Rate Register: selects the division factor for the bit rate generator
              // TWBR = 72
              // SCL freq = F_CPU / (16 + 2 * TWBR * PRESCALER)
              // SCL freq = 16000000 / (16 + 2 * 72 * 1)
              // SCL freq = 16000000 / (16 + 144)
              // SCL freq = 100000kHz
    TWBR = (uint8_t)TWBR_VALUE;
    TWCR = (1 << TWEN); // enable TWI (i don't know if the line is mandatory ?)
}

void i2c_start(void)
{
    // 22.9.2 TWCR - TWI Control Register: used to control the operation of the TWI
    // TWINT - TWI Interrupt Flag: Set when the TWI has finished its current job
    // TWSTA - TWI START Condition Bit: write on this bit if application desire to become a Master
    // TWEN - TWI Enable bit: If set to 1 it enables TWI operation and activates the TWI interface
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    // WHILE TWINT IS NOT SET
    while (!(TWCR & (1<<TWINT))) {} // Wait for start


    // TWSR -TWI Status Register: bit 7 to 3 (read only) reflect the status of the TWI logic
    // bit 1 and 0 control the bit rate prescaler
    // 0xF8 masks the last 3 bits
    // TW_MT_SLA_ACK = SLA+W transmitted, ACK received 
    // if (TWSR & 0xF8 == TW_MT_SLA_ACK || TWSR & 0xF8 == TW_MR_SLA_ACK) 
    //     return 0; // ACK
    // return 1; // NACK
}


void i2c_stop(void)
{
    // TWSTO - TWI STOP Condition Bit: setting TWSTO to 1 in Master mode generates a STOP condition 
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
    while (TWCR & (1 << TWSTO));      // Wait for stop to complete
}


void i2c_write(unsigned char data)
{
    // TWDR - TWI Data Register: in transmit mode TWDR contains the next byte to be transmitted
    // in receive mode, TWDR contains the last byte received 
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)));
}



uint8_t i2c_read(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 >> TWEA);

    while (!(TWCR & (1 << TWINT)));
    return (TWDR);
}

uint8_t i2c_read_nack(void)
{
    TWCR = (1 << TWEN) | (1 << TWINT);

    while (!(TWCR & (1 << TWINT)));
    return (TWDR);
}


 
// maybe rename functions to i2c_exp_ or PCA9555_
void i2c_set_pin(uint8_t addr, uint8_t command, uint8_t port)
{
    i2c_start();
    i2c_write((addr << 1 ) | I2C_WRITE); 
    i2c_write(command);
    i2c_write(port); // re write the whole byte -> probably need to |= and change my defines and enum 
    i2c_stop();
}

uint8_t i2c_get_pin(uint8_t addr, uint8_t command)
{
    uint8_t read = 0;
    i2c_start();
    i2c_write((addr << 1) | I2C_WRITE);
    i2c_write(command);
    i2c_start();
    i2c_write((addr << 1) | I2C_READ);
    read = i2c_read_nack();
    i2c_stop();
    return (read);
}

void i2c_set_pins(uint8_t addr, uint8_t command, uint8_t port0, uint8_t port1)
{
    i2c_start();
    i2c_write((addr << 1 ) | I2C_WRITE); 
    i2c_write(command);
    i2c_write(port0); // re write the whole byte -> probably need to |= and change my defines and enum 
    i2c_write(port1); // re write the whole byte -> probably need to |= and change my defines and enum 
    i2c_stop();
}
// NOTE: Detailed exemple of how is works
void i2c_init_led(uint8_t addr, uint8_t led)
{
    // === Configure pin of 'led' as output ===
    i2c_start();
    i2c_write((addr << 1) | I2C_WRITE); // Address + Write
    i2c_write(CONFIG_PORT_0_COMMAND); // select command Configuration Port 0
    i2c_write(led); // set bit 0 of Port 0 to 0 (output), others = 1 (input)
    i2c_stop();
}

void i2c_blink_led(uint8_t addr, uint8_t led)
{

    // === turn ON only the bit of 'led' ===
    i2c_start();
    i2c_write((addr << 1) | I2C_WRITE); // Address + Write
    i2c_write(OUTPUT_PORT_0_COMMAND); // select command Output Port 0
    i2c_write(led);  // set bit 0 of Port 0 to 0 (ON), others = 1 (OFF)
    i2c_stop();

    _delay_ms(500);


    // === Turn OFF all pins ===
    i2c_start();
    i2c_write((addr << 1) | I2C_WRITE);
    i2c_write(OUTPUT_PORT_0_COMMAND); // select command Output Port 0
    i2c_write(PORT_0_ALL_OFF); // set all to 1 (OFF)
    i2c_stop();

    _delay_ms(500);
}

void i2c_print_reset(uint8_t addr)
{
    /* return ; */

    // Turn off all digits before switching semg
    i2c_set_pins(addr, OUTPUT_PORT_1_COMMAND, PORT_1_ALL_OFF, PORT_0_ALL_OFF);
    _delay_us(50);
}

void i2c_print_digits(uint8_t addr, uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4)
{
    /* while (1) */
    /* { */
    // Digit 1 (segment A OFF)
    i2c_set_pins(addr, OUTPUT_PORT_1_COMMAND, digit1, CA1);
    _delay_ms(2);

    i2c_print_reset(addr);

    // Digit 2 
    i2c_set_pins(addr, OUTPUT_PORT_1_COMMAND, digit2, CA2);
    _delay_ms(2);

    i2c_print_reset(addr);

    // Digit 3
    i2c_set_pins(addr, OUTPUT_PORT_1_COMMAND, digit3, CA3);
    _delay_ms(2);

    i2c_print_reset(addr);

    // Digit 4 
    i2c_set_pins(addr, OUTPUT_PORT_1_COMMAND, digit4, CA4);
    _delay_ms(2);

    i2c_print_reset(addr);
    /* } */
}

void i2c_print_digits_led(uint8_t addr, uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4, uint8_t ledState)
{
    /* while (1) */
    /* { */
    // Digit 1 (segment A OFF)
    uint8_t ca[4];
    if (ledState)
    {
        ca[0] = CA1 & ~LED_D9;
        ca[1] = CA2 & ~LED_D9;
        ca[2] = CA3 & ~LED_D9;
        ca[3] = CA4 & ~LED_D9;
    }
    else
    {
        ca[0] = CA1;
        ca[1] = CA2;
        ca[2] = CA3;
        ca[3] = CA4;
    }
    i2c_set_pins(addr, OUTPUT_PORT_1_COMMAND, digit1, ca[0]);
    _delay_ms(2);

    i2c_print_reset(addr);

    // Digit 2 
    i2c_set_pins(addr, OUTPUT_PORT_1_COMMAND, digit2, ca[1]);
    _delay_ms(2);

    i2c_print_reset(addr);

    // Digit 3
    i2c_set_pins(addr, OUTPUT_PORT_1_COMMAND, digit3, ca[2]);
    _delay_ms(2);

    i2c_print_reset(addr);

    // Digit 4 
    i2c_set_pins(addr, OUTPUT_PORT_1_COMMAND, digit4, ca[3]);
    _delay_ms(2);

    i2c_print_reset(addr);
    /* } */
}


void i2c_display_number(uint8_t addr, uint8_t number)
{
    i2c_set_pin(addr, OUTPUT_PORT_1_COMMAND, number);
}

void    i2c_multiwrite(uint8_t *data, uint8_t size)
{
    for (int i = 0; i < size; i++) {
        i2c_write(data[i]);
    }
}

void    i2c_multiread(uint8_t *data, uint8_t size)
{
        for (int i = 0; i < size; i++) {
        data[i] = i2c_read();
    }
}
