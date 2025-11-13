#include "main.h"

// 8.4 EEPROM Data Memory

/*
   In ATmega328, the address is a 16-bit (4-digit hex) number
   There are in total 1024 memory location inside the data memory. Each memory location can
   hold 8-bit data called byte. In Computer Science 1024 is taken as K; so, the capacity of the data
   memory is 1K.
   */

uint8_t check_input_format(char *buffer)
{
    uint16_t i = 0;
    uint8_t wordCount = 0;
    uint8_t inDoubleQuotes = 0;
    uint8_t inSpace = 0;
    if (!buffer || !buffer[i])
        return (0);
    while (buffer[i] && buffer[i] == SPACE_CHAR)
        i++;
    if (!buffer[i])
        return (0);
    wordCount = 1;
    while (buffer[i])
    {
        if (buffer[i] != SPACE_CHAR && inSpace && !inDoubleQuotes)
            wordCount++;
        if (buffer[i] == SPACE_CHAR)
            inSpace = 1;
        else
            inSpace = 0;
        if (buffer[i] == DOUBLE_QUOTE_CHAR)
            inDoubleQuotes = !inDoubleQuotes;
        #ifdef DEBUG 
            putstr_uart("===============\r\n");
            putstr_uart("i: ");
            putnbr_uart(i);
            putstr_uart(NEW_LINE);
            putstr_uart("char: ");
            write_uart(buffer[i]);
            putstr_uart(NEW_LINE);
            putstr_uart("inQuotes: ");
            putnbr_uart(inDoubleQuotes);
            putstr_uart(NEW_LINE);
            putstr_uart("inSpace: ");
            putnbr_uart(inSpace);
            putstr_uart(NEW_LINE);
            putstr_uart("wordCount: ");
            putnbr_uart(wordCount);
            putstr_uart(NEW_LINE);
        #endif
        i++;
    }
    if (wordCount > MAX_WORD_COUNT || inDoubleQuotes)
        return (0);
    return (1);
}

uint8_t split_input(char *buffer, char **input)
{
    uint16_t i = 0;
    uint16_t a = 0;
    uint8_t inSpace = 0;
    uint8_t inDoubleQuotes = 0;
    uint8_t wordCount = 0;
    while (buffer[i] && buffer[i] == SPACE_CHAR)
        i++;
    wordCount = 1;
    while (buffer[i])
    {
        if (buffer[i] != SPACE_CHAR && inSpace && !inDoubleQuotes)
        {
            input[wordCount][a] = '\0';
            wordCount++;
            a = 0;
        }
        if (buffer[i] == SPACE_CHAR && !inDoubleQuotes)
            inSpace = 1;
        else
            inSpace = 0;
        if (buffer[i] == DOUBLE_QUOTE_CHAR)
            inDoubleQuotes = !inDoubleQuotes;
        if (!inSpace)
        {
            input[wordCount - 1][a] = buffer[i];
            a++;
        }
        i++;
    }
    return (0);
}

// "key" --> key
char *remove_doublequotes(char *str)
{
    uint16_t i = 0;
    if (str[i] && str[i] == DOUBLE_QUOTE_CHAR)
        str += 1;
    while (str[i])
    {
        if (str[i] == DOUBLE_QUOTE_CHAR)
        {
            str[i] = 0;
            break;
        }
        i++;
    }
    return (str);
}

uint32_t check_key_exists(char *key)
{
    uint32_t addr = 0;
    char chunk[CHUNK_SIZE + 1];
    while (addr < MAX_EEPROM_ADDRESS)
    {
        if (EEPROM_read(addr) == MAGIC_VALUE)
        {
            EEPROM_read_chunk(addr + 1, chunk, CHUNK_SIZE);
            #ifdef DEBUG
                putstr_uart("check_key_exists(): \r\n");
                putstr_uart("chunk: ");
                putstr_uart(chunk);
                putstr_uart(NEW_LINE);
                putstr_uart("key: ");
                putstr_uart(key);
                putstr_uart(NEW_LINE);
            #endif
            // read chunk for key
            if (!ft_strcmp(chunk, key))
                return (addr); // return addr of magic value
        }
        addr += CHUNK_SIZE * 2 + 1;
    }
    return (INVALID_EEPROM_ADDR);
}

uint8_t execute_write(char *key, char *value)
{
    uint32_t addr = 0;
    if (!key || !key[0] || !value || !value[0])
        return (putstr_uart(WRONG_INPUT), 0);
    if (check_key_exists(key) != INVALID_EEPROM_ADDR)
        return (putstr_uart(ALREADY_EXISTS),0);
    while (addr < MAX_EEPROM_ADDRESS && EEPROM_read(addr) == MAGIC_VALUE)
        addr += CHUNK_SIZE * 2 + ADDR_SIZE;
    if (addr >= MAX_EEPROM_ADDRESS || (addr + CHUNK_SIZE * 2) >= MAX_EEPROM_ADDRESS)
        return (putstr_uart(NO_SPACE_LEFT), 0);
    EEPROM_write(addr, MAGIC_VALUE);
    addr++;
    EEPROM_write_chunk(addr, key, CHUNK_SIZE);
    addr += CHUNK_SIZE;
    EEPROM_write_chunk(addr, value, CHUNK_SIZE);
    #ifdef DEBUG
            putstr_uart("KEY and VALUE written in EEPROM\r\n");
    #endif
    return (0);
}

uint8_t execute_read(char *key)
{
    char chunk[CHUNK_SIZE + 1];
    uint32_t addr = 0;
    if (!key || !key[0])
        return (putstr_uart(WRONG_INPUT), 0);
    addr = check_key_exists(key);
    if (addr == INVALID_EEPROM_ADDR)
        return (putstr_uart(EMPTY), 0);
    addr += CHUNK_SIZE + 1; // to get the value addr
    putstr_uart(EEPROM_read_chunk(addr, chunk, CHUNK_SIZE));
    putstr_uart(NEW_LINE);
    return (0);
}

uint8_t execute_print(void)
{
    EEPROM_print_c(START_EEPROM_ADDRESS, MAX_EEPROM_ADDRESS, INVALID_EEPROM_ADDR, NB_BYTE_PER_LINE);    
    return (0);
}

uint8_t execute_forget(char *key)
{
    uint16_t addr = 0;
    if (!key || !key[0])
        return (putstr_uart(WRONG_INPUT), 0);
    addr = check_key_exists(key);
    if (addr == INVALID_EEPROM_ADDR)
        return (putstr_uart(NOT_FOUND), 0);
    EEPROM_write(addr, 0);
    #ifdef DEBUG
        putstr_uart("key forgotten\r\n");
    #endif
    return (0);
}

uint8_t check_command(char **input)
{
    if (!ft_strcmp(input[CMD_INPUT], WRITE_COMMAND))
        return (execute_write(input[KEY_INPUT], input[VALUE_INPUT]), 1);
    else if (!ft_strcmp(input[CMD_INPUT], READ_COMMAND))
        return (execute_read(input[KEY_INPUT]), 1);
    else if (!ft_strcmp(input[CMD_INPUT], PRINT_COMMAND))
        return (execute_print(), 1);
    else if (!ft_strcmp(input[CMD_INPUT], FORGET_COMMAND))
        return (execute_forget(input[KEY_INPUT]), 1);
    return (0);
}


int main(void)
{
    char *input[4];
    char command[STR_MAX_SIZE];
    char key[STR_MAX_SIZE];
    char value[STR_MAX_SIZE];
    char buffer[INPUT_BUFFER_SIZE];

    input[CMD_INPUT] = command;
    input[KEY_INPUT] = key;
    input[VALUE_INPUT] = value;
    input[3] = NULL;
    init_uart();

    while (1)
    {
        ft_bzero(buffer, INPUT_BUFFER_SIZE);
        ft_bzero(command, STR_MAX_SIZE);
        ft_bzero(key, STR_MAX_SIZE);
        ft_bzero(value, STR_MAX_SIZE);
        putstr_uart("> ");
        get_input_uart(buffer, INPUT_BUFFER_SIZE);
        if (!check_input_format(buffer))
        {
            putstr_uart(WRONG_INPUT);
            continue ;
        }
        split_input(buffer, input);
        input[KEY_INPUT] = remove_doublequotes(input[KEY_INPUT]);
        input[VALUE_INPUT] = remove_doublequotes(input[VALUE_INPUT]);
        #ifdef DEBUG
            uint16_t i = 0;
            while (input[i])
            {
                putstr_uart("input: ");
                putstr_debug_uart(input[i], '|');
                putstr_uart(NEW_LINE);
                i++;
            }
        #endif
        if (!check_command(input))
        {
            putstr_uart(UNKNOWN_COMMAND);
        }
    }
}

