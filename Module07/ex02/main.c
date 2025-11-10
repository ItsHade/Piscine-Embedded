#include "main.h"

// 8.4 EEPROM Data Memory

/*
   In ATmega328, the address is a 16-bit (4-digit hex) number
   There are in total 1024 memory location inside the data memory. Each memory location can
   hold 8-bit data called byte. In Computer Science 1024 is taken as K; so, the capacity of the data
   memory is 1K.
   */
/* uint8_t input_to_uint8(char *input) */
/* { */
/*     uint8_t hex = 0; */
/*     uint8_t i = 0; */
/*     uint8_t size = ft_strlen(input); */

/*     while (i < size) */
/*     { */
/*         if (ft_isdigit(input[i])) */
/*             hex = hex * 16 + input[i] - HEX_TO_DIGIT; */
/*         else */
/*             hex = hex * 16 + input[i] - HEX_TO_UPPERCASE; */
/*         i++; */
/*     } */
/*     return (hex); */
/* } */

/* uint32_t input_to_uint32(char *input) */
/* { */
/*     uint32_t hex = 0; */
/*     uint8_t i = 0; */
/*     uint8_t size = ft_strlen(input); */

/*     while (i < size) */
/*     { */
/*         if (ft_isdigit(input[i])) */
/*             hex = hex * 16 + input[i] - HEX_TO_DIGIT; */
/*         else */
/*             hex = hex * 16 +  input[i] - HEX_TO_UPPERCASE; */
/*         i++; */
/*     } */
/*     return (hex); */
/* } */

/* uint8_t check_input(char **input) */
/* { */
/*     input[ADDR_INPUT] = ft_capitalize(input[ADDR_INPUT]); */
/*     input[BYTE_INPUT] = ft_capitalize(input[BYTE_INPUT]); */
/*     if (ft_strlen(input[ADDR_INPUT]) > ADDR_INPUT_SIZE || ft_strlen(input[BYTE_INPUT]) != BYTE_INPUT_SIZE) */
/*         return (0); */
/*     if (!ft_ishexchar(input[ADDR_INPUT]) || !ft_ishexchar(input[BYTE_INPUT])) */
/*         return (0); */
/*     return (1); */ 
/* } */

char *get_input(char *input)
{
    char c = 0;
    uint8_t i = 0;
    while (1)
    {
        c = read_uart();
        if (c == '\r')
        {
            input[i] = '\0';
            break;
        }
        else if (c == BACKSPACE_CHAR)
        {
            if (i > 0)
            {
                putstr_uart("\b \b");
                i--;
            }
        }
        else if (i < INPUT_BUFFER_SIZE)
        {
            input[i] = c;
            i++;
        }
        write_uart(c);
    }
    putstr_uart(NEW_LINE);
    return (input);
}

uint8_t split_input(char *buffer, char **input)
{
    uint8_t i = 0;
    uint8_t a = 0;
    // COMMAND
    while (buffer[i] && buffer[i] != SPACE_CHAR)
    {
        input[CMD_INPUT][a] = buffer[i];
        i++;
        a++;
    }
    input[CMD_INPUT][a] = 0;
    if (!buffer[i] || !buffer[i + 1] || buffer[i + 1] != DOUBLE_QUOTE_CHAR)
        return (putstr_uart("1\r\n"), 1);
    a = 0;
    i += 2;
    // KEY
    while (buffer[i] && buffer[i] != DOUBLE_QUOTE_CHAR)
    {
        input[KEY_INPUT][a] = buffer[i];
        i++;
        a++;
    }
    input[KEY_INPUT][a] = 0;
    // VALUE
    if (!buffer[i] || !buffer[i + 1] || buffer[i + 1] != SPACE_CHAR) // can combine with next if
        return (putstr_uart("3\r\n"), 1);
    a = 0;
    i += 2;
    if (!buffer[i] || buffer[i] != DOUBLE_QUOTE_CHAR)
        return (putstr_uart("4\r\n"), 1);
    i++;
    while (buffer[i] && buffer[i] != DOUBLE_QUOTE_CHAR)
    {
        input[VALUE_INPUT][a] = buffer[i];
        i++;
        a++;
    }
    input[VALUE_INPUT][a] = 0;
    if (buffer[++i])
        return (putstr_uart("5\r\n"), 1);
    return (0);
}

// uint8_t check_input(input)
// {
//     if (ft_strcmp(input[0], WRITE_CMD)
// }

int main(void)
{
    char *input[4];
    char command[STR_MAX_SIZE];
    char key[STR_MAX_SIZE];
    char value[STR_MAX_SIZE];
    char buffer[INPUT_BUFFER_SIZE];

    input[0] = command;
    input[1] = key;
    input[2] = value;
    input[3] = NULL;
    /* uint32_t addr = 0; */
    uint8_t i = 0;
    init_uart();

    while (1)
    {
        ft_bzero(buffer, INPUT_BUFFER_SIZE);
        ft_bzero(command, STR_MAX_SIZE);
        ft_bzero(key, STR_MAX_SIZE);
        ft_bzero(value, STR_MAX_SIZE);
        get_input(buffer);
        if (split_input(buffer, input))
        {
            putstr_uart(WRONG_INPUT);
            continue;
        }
        // TEST
        i = 0;
        while (input[i])
        {
            putstr_uart("[DEBUG] Input: ");
            putstr_uart(input[i]);
            putstr_uart(NEW_LINE);
            i++;
        }
        if (check_input(input))
        {
            putstr_uart(WRONG_INPUT);
            continue;
        }
    }
}

