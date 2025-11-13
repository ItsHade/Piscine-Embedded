#include "main.h"

uint8_t EEPROM_read(uint16_t address)
{
    // 8.6.3 EEPROM Control Register
    // Need to wait for EEPE (write enable) to become 0 before reading (check that no write operation is ongoing)
    while (EECR & (1 << EEPE));

    // 8.6.1 EEPROM Address Register
    // Setup the address
    EEAR = address;

    // 8.6.3 EEPROM Control Register
    // Set EERE (read enable) to 1 to trigger the EEPROM Read
    // NOTE: When EEPROM is read, the CPU is halted for four cycles before executing next instruction
    EECR |= (1 << EERE);

    // 8.6.2 EEPROM Data Register
    // used to write or read data from EEPROM
    return EEDR;
}

void EEPROM_write(uint16_t address, uint8_t byte)
{
    // 8.6.3 EEPROM Control Register
    // Need to wait for EEPE (write enable) to become 0 before writing (check that no write operation is ongoing)
    while (EECR & (1 << EEPE));

    // Setup address AND Data register
    EEAR = address;
    EEDR = byte;

    // 8.6.3 EEPROM Control Register
    // Setting EEMPE to 1 determines if setting EEPE to 1 causes the EEPROM to be written (if not set, setting EEPE to 1 does nothing)
    EECR |= (1 << EEMPE);
    
    // 8.6.3 EEPROM Control Register
    // Set EEPE (write enable) to 1 to trigger the EEPROM write 
    EECR |= (1 << EEPE);
}

void EEPROM_write_chunk(uint16_t addr, char *chunk, uint16_t chunkSize)
{
    uint16_t i = 0;
    while (addr < MAX_EEPROM_ADDRESS && i < chunkSize && chunk[i])
    {
        EEPROM_write(addr, chunk[i]); 
        i++;
        addr++;
    }
    while (addr < MAX_EEPROM_ADDRESS && i< chunkSize)
    {
        EEPROM_write(addr, 0);
        i++;
        addr++;
    }
}

char *EEPROM_read_chunk(uint16_t addr, char *chunk, uint16_t chunkSize)
{
    uint16_t i = 0;
    while (addr < MAX_EEPROM_ADDRESS && i < chunkSize)
    {
        chunk[i] = EEPROM_read(addr);
        addr++;
        i++;
    }
    chunk[i] = 0;
    return (chunk);
}

void EEPROM_print(uint16_t startAddr, uint16_t stopAddr, uint32_t highlight, uint16_t nbBytesPerLine)
{
    uint16_t addr = startAddr;
    uint8_t a = 0;
    // 8.4 EEPROM Data Memory
    // The ATmega328P contains 1Kbytes of data EEPROM memory (also cf Table 28-12)
    // 1 KB = 1024 Bytes (in binary)
    // MAX_ADDRESS is 1023 --> 0 - 1023 = 1024
    while (addr < stopAddr)
    {
        a = 0;
        puthex_lower_uart(0x00);// padding for larger addresses 
        puthex_lower_uart(0x00);// padding for larger addresses 
        puthex_lower_uart((uint8_t)(addr >> 8));
        puthex_lower_uart((uint8_t)addr);
        write_uart(' ');
        while (a++ < nbBytesPerLine)
        {
            if (addr == highlight)
            {
                putstr_uart(TEXT_COLOR_RED);
                puthex_lower_uart(EEPROM_read(addr));
                putstr_uart(TEXT_COLOR_RESET);
            }
            else
                puthex_lower_uart(EEPROM_read(addr));
            if (addr % 2)
                write_uart(' ');
            addr++;
        }
        putstr_uart(NEW_LINE);
    }
}


void EEPROM_print_c(uint16_t startAddr, uint16_t stopAddr, uint32_t highlight, uint16_t nbBytesPerLine)
{
    uint16_t addr = startAddr;
    uint8_t a = 0;
    char buffer[nbBytesPerLine + 1];
    uint8_t read = 0;
    // 8.4 EEPROM Data Memory
    // The ATmega328P contains 1Kbytes of data EEPROM memory (also cf Table 28-12)
    // 1 KB = 1024 Bytes (in binary)
    // MAX_ADDRESS is 1023 --> 0 - 1023 = 1024
    while (addr < stopAddr)
    {
        a = 0;
        puthex_lower_uart(0x00);// padding for larger addresses 
        puthex_lower_uart(0x00);// padding for larger addresses 
        puthex_lower_uart((uint8_t)(addr >> 8));
        puthex_lower_uart((uint8_t)addr);
        putstr_uart("  ");
        while (a < nbBytesPerLine)
        {
            read = EEPROM_read(addr);
            if (addr == highlight)
            {
                putstr_uart(TEXT_COLOR_RED);
                puthex_lower_uart(read);
                putstr_uart(TEXT_COLOR_RESET);
            }
            else
                puthex_lower_uart(read);
            if (ft_isprint(read))
                buffer[a] = read;
            else
                buffer[a] = NON_PRINT;

            write_uart(' ');
            if (a == nbBytesPerLine / 2 || a == nbBytesPerLine - 1)
                write_uart(' ');
            addr++;
            a++;
        }
        buffer[a] = 0;
        write_uart('|');
        putstr_uart(buffer);
        write_uart('|');
        putstr_uart(NEW_LINE);
    }
}
