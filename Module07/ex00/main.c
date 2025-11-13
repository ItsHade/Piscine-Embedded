#include "main.h"

// 8.4 EEPROM Data Memory

/*
In ATmega328, the address is a 16-bit (4-digit hex) number
 There are in total 1024 memory location inside the data memory. Each memory location can
hold 8-bit data called byte. In Computer Science 1024 is taken as K; so, the capacity of the data
memory is 1K.
*/



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


int main(void)
{
    init_uart();
    uint16_t addr = 0;
    uint8_t a = 0;
    // 8.4 EEPROM Data Memory
    // The ATmega328P contains 1Kbytes of data EEPROM memory (also cf Table 28-12)
    // 1 KB = 1024 Bytes (in binary)
    // MAX_ADDRESS is 1023 --> 0 - 1023 = 1024
    while (addr < MAX_ADDRESS)
    {
        a = 0;
        puthex_lower_uart(0x00);// padding for larger addresses 
        puthex_lower_uart(0x00);// padding for larger addresses 
        puthex_lower_uart((uint8_t)(addr >> 8));
        puthex_lower_uart((uint8_t)addr);
        write_uart(' ');
        while (a++ < NB_BYTE_PER_LINE)
        {
            puthex_lower_uart(EEPROM_read(addr));
            if (addr % 2)
                write_uart(' ');
            addr++;
        }
        putstr_uart(NEW_LINE);
    }

    // 28.8 Serial Downloading
    // " The Chip Erase operation turns the content of every memory location in both the Program and EEPROM arrays into 0xFF."
    // EEPROM default value for all bytes is 0xFF 
}

