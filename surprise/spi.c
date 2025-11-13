#include "main.h"

// 19 SPI - Serial Peripheral Interface

void spi_init(void)
{
    // Setup SS, MOSI and SCK as output (only need those 2 for RGB communication)
    DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB5);

    // 19.5.1 SPCR - SPI Control Register
    // Enable SPI
    // Select Master SPI mode
    // Select SPI clock rate
    // Fosc / 128 (safe)
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1); 
    
    // Don't use interrupt for now (SPIE)
    
    // DORD set to 0 (default value) = MSB transmitted first (1 = LSB transmitted first)
    
    // CPOl and CPHA stay at 0 -> Leading edge rising and Leagind edge -> sample
}

void spi_close(void)
{
    SPCR &= ~(1 << SPE);
}

void spi_write(uint8_t data)
{
    // 19.5.3 SPDR - SPI Data Register
    SPDR = data;
    // Wait for transfer to be completed
    while (!(SPSR & (1 << SPIF)));
}

// Start frame 32 bits (only bits of value 0)
void spi_start(void)
{
    uint8_t i = 0;
    while (i < 4)
    {
        spi_write(SPI_START_FRAME_BYTE);
        i++;
    }
}

// not the same as close !!
// End frame 32 bits (only bits of value 1)
void spi_end(void)
{
    uint8_t i = 0;
    while (i < 4)
    {
        spi_write(SPI_END_FRAME_BYTE);
        i++;
    }
}

// LED Frame 32 bits -> (3 bits of value 1, 5bits brightness, 8bits for blue, 8bits for green, 8bits for red)  
// first param only bit 0 to 4
void spi_set_led_all(uint8_t brightness, uint8_t color[3][3])
{
    uint8_t led = 0;
    spi_start();
    while (led < 3)
    {   
        spi_write(brightness);
        spi_write(color[led][2]);
        spi_write(color[led][1]);
        spi_write(color[led][0]);
        led++;
    }
    spi_end();

}

// will set both other leds to OFF
void spi_set_led(uint8_t led, uint8_t brightness, uint8_t color[3])
{
    uint8_t ledFrame = 0;
    spi_start();
    while (ledFrame < 3)
    {
        if (ledFrame == led)
        {
            spi_write(brightness);
            spi_write(color[2]);
            spi_write(color[1]);
            spi_write(color[0]);
        }
        else
        {
            spi_write(0xE0);
            spi_write(0x00);
            spi_write(0x00);
            spi_write(0x00);
        }
        ledFrame++;
    }
    spi_end();
}


