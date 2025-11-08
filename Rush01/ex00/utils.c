#include "main.h"

uint16_t ft_strlen(char *s)
{
    uint16_t i = 0;
    while (*s++)
        i++;
    return (i);
}


uint8_t ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

uint8_t ft_is_allowed_chars(char *s, char *allowed)
{
    uint8_t i = 0;
	uint8_t a = 0;
    while (s[i])
    {
		a = 0;
		while (allowed[a])
		{
			if (s[i] == allowed[a])
				break;
			a++;
		}
		if (allowed[a] == '\0')
			return (0);
		i++;
	}
	return (1);
}

char *ft_capitalize(char *str)
{
	uint8_t i = 0;
	while (str[i])
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] -= ('a' - 'A');
		i++;
	}
	return (str);
}

void ft_bzero(void *s, uint16_t n)
{
	unsigned char *p = (unsigned char *)s;
	uint16_t i = 0;
	while (i < n)
	{
		p[i] = 0;
		i++;
	}
}
