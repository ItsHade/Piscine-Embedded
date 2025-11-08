#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <util/twi.h>
#include "main.h"

// PAGE 6 - Registers
// PAGE 8 to 10


void init_led(void)
{
    DDRB |= (1 << PB0);   // LED output
    PORTB &= ~(1 << PB0); // LED off

    DDRB |= (1 << PB1);   // LED output
    PORTB &= ~(1 << PB1); // LED off

    DDRB |= (1 << PB2);   // LED output
    PORTB &= ~(1 << PB2); // LED off

    DDRB |= (1 << PB4);   // LED output
    PORTB &= ~(1 << PB4); // LED off

    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);
}

void clear_countdown_led()
{
    BitClr(PORTB, PB0);
    BitClr(PORTB, PB1);
    BitClr(PORTB, PB2);
    BitClr(PORTB, PB4);
}


void clear_winner_led()
{
    BitClr(PORTD, PD5);
    BitClr(PORTD, PD3);
    BitClr(PORTD, PD6);
}

void set_winner_led()
{

    PORTD |= TOGGLE_GREEN;
}

void set_looser_led()
{
    PORTD |= TOGGLE_RED;
}

void init_switch(void)
{
    DDRD |= (1 << PD2); // Input (default value)
    PORTD |= (1 << PD2); // high
}


uint8_t check_ready(void)
{
    if (!(PIND & (1 << PD2)))
        return (1);
    return (0);
}

void player_ready(void)
{
    PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
}

void player_not_ready(void)
{
    PORTB &= ~(1 << PB0);
    PORTB &= ~(1 << PB1);
    PORTB &= ~(1 << PB2);
    PORTB &= ~(1 << PB4);
}


uint8_t start_successful(void)
{
    // Send START condition
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    // Wait for it to complete
	TWI_wait();

    // Check the status code
    uint8_t status = TWI_STATUS;

    // START = 0x08, repeated START = 0x10
    if (status == 0x08 || status == 0x10) {
        return 1;  // success
    } else {
        // Clear START condition (release bus)
        TWCR = (1 << TWINT) | (1 << TWEN);
        return 0;  // failed
    }
}

uint8_t slave_game_started(uint8_t counter)
{
    counter += check_timer(counter);
    return (counter);
}


uint8_t slave_listen(uint8_t *slaveReady, uint8_t *gameStarted, uint8_t *counter)
{
    uint8_t send_bit = *slaveReady;

    if (*gameStarted)
        *counter = slave_game_started(*counter);

    TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
    // // Wait for an event
    if (!TWI_wait_until(TIMEOUT))
    {
        return (0);
    }
    
    switch (TWSR & 0xF8)
    {
        case 0xA8: // SLA + R received; ACK returned
        case 0xB8:
            TWDR = send_bit;
            TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
            break;
        case 0x60: // Own SLA+W received; ACK returned
            TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
            break;
        case 0xC0:
        case 0xC8: // Last data transmitted, ACK received
            // Transmission done
            TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
            break;
        case 0x80: // Data received; ACK returned
            if (TWDR == START_COUNTDOWN_BIT) {
                BitSet(TIFR1, TOV1);
                TCNT1 = TIMER_VALUE;
                *slaveReady = 0;
                *gameStarted = 1;
            }
            else if (TWDR == SLAVE_WINNER_BIT) {
                set_winner_led();
                *slaveReady = 0; 
            }
            else if (TWDR == MASTER_WINNER_BIT) {
                set_looser_led();
                *slaveReady = 0;
            }
            TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
            break;

        case 0xA0: // STOP or repeated START received
            TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
            break;

        default:
            TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
            break;
    }
    return (1);
}



void master_loop(void)
{
    uint8_t masterReady = 0;
    uint8_t slaveReady = 0;
    uint8_t gameStarted = 0;
    uint8_t counter = 0;
    uint8_t send = READ; 
    uint8_t sent_bit = START_COUNTDOWN_BIT;
    uint8_t winner = 2;

    while(1)
    {
        if (gameStarted)
        {
            counter += check_timer(counter);
        }
        if (masterReady && slaveReady)
        {
            send = WRITE;
            gameStarted = 1;
            masterReady = 0;
            slaveReady = 0;
        }
        if (check_ready() == 1 && !gameStarted)
        {
            masterReady = 1;
            player_ready();
        }
        if (check_ready() && counter >= 4 && gameStarted && winner == NONE)
        {
            send = WRITE;
            sent_bit = MASTER_WINNER_BIT;
            winner = MASTER;
            set_winner_led();
        }
        if (!TWI_start())
        {
            TWI_stop();
            return ;
        }
        if (send == READ)
        {
            if (!TWI_write((MICRO_SLAVE_ADDR << 1) | READ)) // send addr
            {
                TWI_stop();
                return ;
            }
            uint8_t c = TWI_read_no_ack();
            switch (c)
            {
                case ERROR_CODE:
                    TWI_stop();
                    return;
                case SLAVE_READY_BIT:
                    slaveReady = 1;
                    break;
                case SLAVE_WINNER_BIT:
                    send = WRITE;
                    sent_bit = SLAVE_WINNER_BIT;
                    winner = SLAVE;
                    set_looser_led();
                    break;
                default:
                    break;
            }
        }
        else if (send == WRITE)
        {
            if (!TWI_write((MICRO_SLAVE_ADDR << 1) | WRITE)) // send addr
            {
                TWI_stop();
                return ;
            }
            if (!TWI_write(sent_bit))
            {
                TWI_stop();
                return;
            }
            if (sent_bit == START_COUNTDOWN_BIT)
            {
                BitSet(TIFR1, TOV1);
                TCNT1 = TIMER_VALUE;
                sent_bit = MONITORING_BIT;
                send = READ;
            }
            if (sent_bit == MASTER_WINNER_BIT || sent_bit == SLAVE_WINNER_BIT)
                sent_bit = MONITORING_BIT;

        }
    }
}

void slave_loop(void)
{
    uint8_t slaveReady = 0;
    uint8_t gameStarted = 0;
    uint8_t counter = 0;

    while (1)
    {
        if (gameStarted == 0 && check_ready())
        {
            slaveReady = SLAVE_READY_BIT;
            player_ready();
        }
        if (gameStarted && counter >= 4 && check_ready())
        {
            slaveReady = SLAVE_WINNER_BIT;
        }
        if (!slave_listen(&slaveReady, &gameStarted, &counter))
			break ;
    }
}

int main(void) {
    _delay_ms(10);
    init_led();
    init_switch();
    init_timer();

    while (1)
    {
        clear_countdown_led();
        clear_winner_led();
		TWI_init_master();
		master_loop();
		_delay_ms(50);
        clear_countdown_led();
        clear_winner_led();
		TWI_init_slave();
		slave_loop();

    }
}
