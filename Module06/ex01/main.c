#include "main.h"

// AHT20 Datasheet
// Trigger measurement data
// Read temperature and humidity data

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
    TWCR = (1 << TWEN);
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

void print_hex_value(char c)
{
    uint8_t hex = (uint8_t)c;
    const char *hexa = "0123456789ABCDEF";
    write_uart(hexa[hex >> 4]);
    write_uart(hexa[hex & 0x0F]);
    write_uart(' ');
}


void i2c_write(unsigned char data)
{
    // TWDR - TWI Data Register: in transmit mode TWDR contains the next byte to be transmitted
    // in receive mode, TWDR contains the last byte received 
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)));
}


uint8_t i2c_get_nack(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)));
    return (TWDR);
}

void i2c_read(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

    while (!(TWCR & (1 << TWINT)));
    print_hex_value(TWDR);
}

void i2c_read_nack(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN); // no ack
    while (!(TWCR & (1 << TWINT)));
    print_hex_value(TWDR);
}

int main(void)
{
    uint8_t status;
    init_uart();
    i2c_init();
    _delay_ms(40); // 5.4 Sensor reading process (wait for 40ms after power-on)

    i2c_start();
    i2c_write((AHT20_ADDR << 1) | I2C_WRITE);
    i2c_write(0x71); // get a byte of status word --> to check if the calibration enable bit of the status word is 1
                     // if not 1, send 0xbe command (for initialization) with 2 data byte: 0x08, 0x00
    i2c_stop();
    _delay_ms(10);

    i2c_start();
    i2c_write((AHT20_ADDR << 1) | I2C_READ);
    status = i2c_get_nack(); //read but i return the value
    i2c_stop();

    // if bit 3 of status word is not 1 (= not enabled)
    if (!(status & 0x08))
    {
        i2c_start();
        i2c_write((AHT20_ADDR << 1) | I2C_WRITE);
        i2c_write(0xBE);
        i2c_write(0x08);
        i2c_write(0x00);
        i2c_stop();
        _delay_ms(10); // send 0xbe command for initialization ... then wait for 10ms (datasheet)
    }


    while (1)
    {
        // trigger measurement data
        i2c_start();
        i2c_write((AHT20_ADDR << 1 ) | I2C_WRITE);
        i2c_write(0xAC);
        i2c_write(0x33);
        i2c_write(0x00);
        i2c_stop();
        _delay_ms(80); // wait for 80ms for the measurement to be completed (datasheet)

        // read temperature and humidity data
        i2c_start();
        i2c_write((AHT20_ADDR << 1) | I2C_READ);
        i2c_read(); // state
        i2c_read(); // humidity data
        i2c_read(); // humidity data
        i2c_read(); // humidity temperature 
        i2c_read(); // temperature data
        i2c_read(); // temperature data
        i2c_read_nack(); // CRC data
        i2c_stop();
        putstr_uart(NEW_LINE);
        _delay_ms(2000); // 2.4 it is recommended to measure data every 2 seconds
    }
}
