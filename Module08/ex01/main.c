#include "main.h"

int main(void)
{
    uint8_t colorIndex = 0;
    uint8_t colors[SPI_COLOR_COUNT][3] = {SPI_COLOR_RED, SPI_COLOR_GREEN, SPI_COLOR_BLUE, SPI_COLOR_YELLOW, SPI_COLOR_CYAN, SPI_COLOR_MAGENTA, SPI_COLOR_WHITE};
    spi_init();
    while (1)
    {
        if (colorIndex >= SPI_COLOR_COUNT)
            colorIndex = 0;
        spi_set_led(LED_D6, SPI_BRIGHTNESS_MAX, colors[colorIndex]);
        colorIndex++;
        _delay_ms(1000);
    }
    spi_close();
}

