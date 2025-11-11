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

uint8_t ft_is_allowed_char(char c, char *allowed)
{
	uint8_t i = 0;
	while (allowed[i])
	{
		if (c == allowed[i])
			return (1);
		i++;
	}
	return (0);
}

uint8_t ft_is_allowed_chars(char *s, char *allowed)
{
    uint16_t i = 0;
    while (s[i])
    {
		if (!ft_is_allowed_char(s[i], allowed))
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

// only takes str[0] and str[1] 
uint8_t ft_atouint8(char *str)
{
	uint8_t i = 0;
	uint8_t nb = 0;
	while (str[i] && i < 2)
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	return (nb);
}


uint16_t ft_strcmp(char *s1, char *s2)
{
	uint16_t i = 0;
	uint16_t a = 0;
	while (s1[i] && s2[a] && s1[i] == s2[a])
	{
		i++;
		a++;
	}
	return (s1[i] - s2[a]);
}


void	*ft_memcpy(void *dest, const void *src, uint8_t n)
{
	uint8_t	i;

	i = 0;
	if (!dest && !src)
		return (NULL);
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}