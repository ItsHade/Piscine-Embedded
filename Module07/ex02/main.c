#include "main.h"

// 8.4 EEPROM Data Memory

/*
   In ATmega328, the address is a 16-bit (4-digit hex) number
   There are in total 1024 memory location inside the data memory. Each memory location can
   hold 8-bit data called byte. In Computer Science 1024 is taken as K; so, the capacity of the data
   memory is 1K.
   */
uint8_t input_to_uint8(char *input)
{
    uint8_t hex = 0;
    uint8_t i = 0;
    uint8_t size = ft_strlen(input);

    while (i < size)
    {
        if (ft_isdigit(input[i]))
            hex = hex * 16 + input[i] - HEX_TO_DIGIT;
        else
            hex = hex * 16 + input[i] - HEX_TO_UPPERCASE;
        i++;
    }
    return (hex);
}

uint32_t input_to_uint32(char *input)
{
    uint32_t hex = 0;
    uint8_t i = 0;
    uint8_t size = ft_strlen(input);

    while (i < size)
    {
        if (ft_isdigit(input[i]))
            hex = hex * 16 + input[i] - HEX_TO_DIGIT;
        else
            hex = hex * 16 +  input[i] - HEX_TO_UPPERCASE;
        i++;
    }
    return (hex);
}

uint8_t check_input(char **input)
{
    input[ADDR_INPUT] = ft_capitalize(input[ADDR_INPUT]);
    input[BYTE_INPUT] = ft_capitalize(input[BYTE_INPUT]);
    if (ft_strlen(input[ADDR_INPUT]) > ADDR_INPUT_SIZE || ft_strlen(input[BYTE_INPUT]) != BYTE_INPUT_SIZE)
        return (0);
    if (!ft_ishexchar(input[ADDR_INPUT]) || !ft_ishexchar(input[BYTE_INPUT]))
        return (0);
    return (1); 
}

char **get_input(char **input)
{
    uint8_t inputNb = 0;
    char c = 0;
    uint8_t i = 0;
    uint8_t oldIndex = 0;
    while (1)
    {
        c = read_uart();
        if (c == '\r')
        {
            input[inputNb][i] = '\0';
            break;
        }
        else if (c == BACKSPACE_CHAR)
        {
            if (i == 0)
            {
                i = oldIndex + 1; // Added 1 because the space is not stored in the input buffer but it will decrement by 1 when it's removed on screen
                inputNb = ADDR_INPUT;
            }
            if (i > 0)
            {
                write_uart('\b');
                write_uart(' ');
                write_uart('\b');
                i--;
            }
        }
        else if (c == ' ' && inputNb == ADDR_INPUT)
        {
            input[inputNb][i] = '\0'; 
            inputNb = BYTE_INPUT;
            oldIndex = i;
            i = 0;
        }
        else if (i < INPUT_BUFFER_SIZE)
        {
            input[inputNb][i] = c;
            i++;
        }
        write_uart(c);
    }
    putstr_uart(NEW_LINE);
    return (input);
}



int main(void)
{
    char *input[2];
    char input1[INPUT_BUFFER_SIZE];
    char input2[INPUT_BUFFER_SIZE];

    input[ADDR_INPUT] = input1;
    input[BYTE_INPUT] = input2;
    uint32_t addr = 0;
    uint8_t byte = 0;
    init_uart();

    while (1)
    {
        ft_bzero(input[ADDR_INPUT], INPUT_BUFFER_SIZE);
        ft_bzero(input[BYTE_INPUT], INPUT_BUFFER_SIZE);
        get_input(input);
        if (!check_input(input)) 
        {
            putstr_uart(WRONG_INPUT);
            putstr_uart(ADDRESS_RANGE);
            continue;
        }
        addr = input_to_uint32(input[ADDR_INPUT]);
        byte = input_to_uint8(input[BYTE_INPUT]);
    
        /* if (EEPROM_read((uint16_t)addr == byte)) */
        /* { */   
        /*     putstr_uart("Value "); */
        /*     puthex_uart(byte); */
        /*     putstr_uart(" is already set at "); */
        /*     puthex_uart(0x00); */
        /*     puthex_uart(0x00); */
        /*     puthex_uart((uint8_t)addr >> 8); */
        /*     puthex_uart((uint8_t)addr); */
        /* } */
        if (addr <= MAX_ADDRESS && EEPROM_read((uint16_t)addr) != byte)
        {
            EEPROM_write((uint16_t)addr, byte);
            EEPROM_print(0, MAX_ADDRESS, (uint16_t)addr); 
        }
        putstr_uart(NEW_LINE);
    }
    // 28.8 Serial Downloading
    // " The Chip Erase operation turns the content of every memory location in both the Program and EEPROM arrays into 0xFF."
    // EEPROM default value for all bytes is 0xFF 
}

