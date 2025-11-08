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
    
    // Forced to send ADDR + W/R bit
    // ADDR + W
    TWDR = ((AHT20_ADDR << 1) | I2C_WRITE);
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

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

int main(void)
{
    init_uart();
    i2c_init();

    i2c_start();
    putstr_uart(STATUS_STR);
    puthex_uart(TWSR & TW_STATUS_MASK); // defined in twi.h as TW_STATUS
    putstr_uart(NEW_LINE);
    i2c_stop();
    putstr_uart(STATUS_STR);
    puthex_uart(TWSR & TW_STATUS_MASK);
    putstr_uart(NEW_LINE);
}

