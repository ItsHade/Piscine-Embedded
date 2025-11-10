#include "main.h"

int main(void)
{
    spi_init();
    
    while (1)
    {
        _delay_ms(20);
    }
    spi_close();
}

