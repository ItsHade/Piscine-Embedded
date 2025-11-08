#include "main.h"
#include <stdint.h>
#include <util/delay.h>
#include <util/twi.h>

int TWI_start_status(void)
{
    uint8_t status = TWI_STATUS;
    switch (status)
	{
		case TW_START:
			return (1);
	
		case TW_REP_START:
			return (2);

		default:
			return (0);
    }
    return (0);
}

int TWI_write_status(void)
{
    uint8_t status = TWI_STATUS;
    switch (status)
	{
		//MASTER TRANSMITTER MODE
		case TW_MT_DATA_ACK: // write
			return (1);

		case TW_MT_SLA_ACK: // write
			return (2);

        case TW_MR_SLA_ACK:
            return (3);

		// SLAVE TRANSMITTER MODE
		case TW_ST_DATA_ACK: // write
			return (3);

		case TW_ST_DATA_NACK: // write
			return (3);

		case TW_ST_LAST_DATA: // write
			return (4);

        case TW_MT_SLA_NACK:
            return (4);

		default:
			return (0);
    }
    return (0);
}

int TWI_read_status(void)
{
    uint8_t status = TWI_STATUS;
    switch (status)
	{
		case TW_MR_SLA_ACK:
			return (1);

		case TW_SR_SLA_ACK:
			return (2);

		case TW_MR_DATA_ACK:
			return (3);

		// SLAVE TRANSMITTER MODE
		case TW_SR_ARB_LOST_SLA_ACK:
			return (4);

		case TW_SR_ARB_LOST_GCALL_ACK:
			return (5);

		case TW_SR_DATA_ACK:
			return (6);

		case TW_SR_DATA_NACK:
			return (6);

		case TW_SR_GCALL_DATA_ACK:
			return (7);

		case TW_SR_GCALL_DATA_NACK:
			return (7);

		case TW_ST_SLA_ACK:
			return (8);
        
        case TW_MR_DATA_NACK:
            return (9);

		default:
			return (0);
    }
    return (0);
}

void TWI_init_slave(void)
{
    // Set slave address (7 bits)
    TWAR = (MICRO_SLAVE_ADDR << 1);
}


void TWI_init_master(void)
{
    TWSR = 0; // Prescaler = 1
    TWBR = (uint8_t)TWBR_VALUE;
}

int TWI_start(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    // WHILE TWINT IS NOT SET
	TWI_wait();
	return (TWI_start_status());
}


uint8_t TWI_read_no_ack(void)
{
    TWCR = (1 << TWEN) | (1 << TWINT);
    TWI_wait();
    if (TWI_read_status() == 0)
        return (ERROR_CODE);
    return (TWDR);		
}

int TWI_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
	TWI_wait();
	return (TWI_write_status());
}

void TWI_stop(void)
{
    // TWSTO - TWI STOP Condition Bit: setting TWSTO to 1 in Master mode generates a STOP condition 
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
    while (BitCheck(TWCR, TWSTO));      // Wait for stop to complete
}

// wait interrupt flag
void TWI_wait(void)
{
    while (!(BitCheck(TWCR, TWINT)));
}

uint16_t TWI_wait_until(uint16_t ms)
{
    while (!(BitCheck(TWCR, TWINT)) && ms != 0)
	{
		ms--;
		_delay_ms(1);
	}
	return (ms);
}
