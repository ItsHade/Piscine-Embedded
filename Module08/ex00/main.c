#include "main.h"

int main(void)
{
    uint8_t color[3] = SPI_COLOR_RED;
    spi_init();
    spi_set_led(LED_D6, SPI_BRIGHTNESS_MAX, color);
    spi_close();
}

