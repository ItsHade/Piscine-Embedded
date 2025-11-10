#include "main.h"

int main(void)
{
    uint16_t read = 0;
    init_adc();
    spi_init();
    
    while (1)
    {
        spi_set_led_all(SPI_BRIGHTNESS_MAX, (uint8_t [3][3]){SPI_COLOR_OFF, SPI_COLOR_OFF, SPI_COLOR_OFF});
        read = read_adc();
        if (read >= MAX_ADC_VALUE / 3)
            spi_set_led_all(SPI_BRIGHTNESS_MAX, (uint8_t [3][3]){SPI_COLOR_MAGENTA, SPI_COLOR_OFF, SPI_COLOR_OFF});
        if (read >= MAX_ADC_VALUE / 3 * 2)
            spi_set_led_all(SPI_BRIGHTNESS_MAX, (uint8_t [3][3]){SPI_COLOR_MAGENTA, SPI_COLOR_MAGENTA, SPI_COLOR_OFF});
        if (read == MAX_ADC_VALUE)
            spi_set_led_all(SPI_BRIGHTNESS_MAX, (uint8_t [3][3]){SPI_COLOR_MAGENTA, SPI_COLOR_MAGENTA, SPI_COLOR_MAGENTA});

        _delay_ms(20);
    }
    spi_close();
}

