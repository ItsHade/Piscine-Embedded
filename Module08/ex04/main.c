#include "main.h"

// a simple counter to set a flag on given freq (counter 1)
void    init_timer1(uint16_t freq_hz)
{
    TCNT1 = 0;
     // Set CS12 to one, modifying clock select.
    // CTC mode ; top will be OCR1A
    TCCR1A |= (1 << WGM12);
    // calculate 
    uint32_t top = (F_CPU / (2UL * TIMER1_PRESCALER * (uint32_t)freq_hz) - 1);
    OCR1A = (uint16_t)top;
    // Enable interrupts on timer 1, then interrupts on CTC match for OCR1a 
    TIMSK1 |= (1 << OCIE1A);
    
    // Now timer 1 will increment every 1024 ticks
    TCCR1B |= (1 << CS11);
}

void stop_timer1(void)
{
    TCCR1B &= ~(1 << CS11);
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t led)
{
    // {LED_D6, LED_D7, LED_D8}
    uint8_t color[3] = {r, g, b};
    static uint8_t colors[3][3] = {SPI_COLOR_OFF, SPI_COLOR_OFF, SPI_COLOR_OFF}; 

    if (led == LED_D6 || led == LED_ALL)
        ft_memcpy(colors[0], color, sizeof(color));
    if (led == LED_D7 || led == LED_ALL)
        ft_memcpy(colors[1], color, sizeof(color));
    if (led == LED_D8 || led == LED_ALL)
        ft_memcpy(colors[2], color, sizeof(color));

    spi_set_led_all(SPI_BRIGHTNESS_MAX, colors);
}

void wheel(uint8_t pos) {
	pos = 255 - pos;
	if (pos < 85) 
	{
		set_rgb(255 - pos * 3, 0, pos * 3, LED_ALL);
	} 
	else if (pos < 170) 
	{
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3, LED_ALL);
	} 
	else 
	{
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0, LED_ALL);
	}
}

FT_ISR(TIMER1_COMPA_vect)
{
    static uint8_t pos = 0;

    wheel(pos);
    pos++;
}

uint8_t ft_ishexstr(char *buffer, uint8_t size)
{
	uint8_t i = 1;
	while (buffer[i] && i < size)
	{
		if (!((buffer[i] >= '0' && buffer[i] <= '9')
			|| (buffer[i] >= 'A' && buffer[i] <= 'F')))
			return (0);
		i++;
	}
	return (1);
}

uint32_t ft_hexatoi(char *str)
{
	uint8_t i = 0;
	uint32_t result = 0;
	char c;
	while (str[i])
	{
		uint8_t digit;
		c = str[i];
        if (c >= '0' && c <= '9')
            digit = c - '0';
        else if (c >= 'A' && c <= 'F')
            digit = c - 'A' + 10;
        else
            break; // stop at first invalid character
        result = (result << 4) | digit;
		i++;
	}
	return (result);
}


uint8_t check_input(char *buffer)
{
	if (buffer[0] == '\0' || buffer[0] != '#')
		return (INPUT_ERROR);
    if (!ft_strcmp(buffer, FULLRAINBOW))
            return (FULLRAINBOW_VALUE);
	if (ft_strlen(buffer) != INPUT_CORRECT_SIZE)
		return (INPUT_ERROR);
	buffer = ft_capitalize(buffer);
    buffer++; // #

	if (!ft_ishexstr(buffer, 6) || buffer[6] != 'D' || !ft_is_allowed_char(buffer[7], "678"))
		return (INPUT_ERROR);
	return (buffer[7] - INT_TO_CHAR_DIGIT);
}

void set_rgb_from_input(char *buffer, uint8_t mode)
{
    buffer++; // #
    buffer[6] = '\0';
    uint8_t led = 0;
    if (mode == FULLRAINBOW_VALUE)
    {
        init_timer1(200);
        return;
    }
    uint32_t color = ft_hexatoi(buffer);
    uint8_t red   = (color >> 16) & 0xFF;
    uint8_t green = (color >> 8)  & 0xFF;
    uint8_t blue  =  color        & 0xFF;

    switch (mode)
    {
    case 6:
        led = LED_D6;
        break;
    case 7:
        led = LED_D7;
        break;
    case 8:
        led = LED_D8;
        break;
    default:
        break;
    }
    set_rgb(red, green, blue, led);

}

int main(void)
{
    char buffer[INPUT_BUFFER_SIZE];
    uint8_t mode = 0;
    spi_init();
    init_uart();
    sei();

    while (1)
    {
        ft_bzero(buffer, INPUT_BUFFER_SIZE);
        get_input_uart(buffer, INPUT_BUFFER_SIZE);
        mode = check_input(buffer);
        if (mode == INPUT_ERROR)
        {
            putstr_uart(WRONG_INPUT);
            continue;
        }
        stop_timer1();
        putstr_uart(NEW_LINE);
        set_rgb_from_input(buffer, mode);
        _delay_ms(20);
    }
    spi_close();
}

