#include <avr/io.h>
#include <stdint.h>

#define CYCLES_PER_LOOP 12  // ~ the number of cycle it take for each iteration of the while loop 

void my_delay_ms(uint32_t ms)
{
    uint32_t count;

    while (ms--) {
        // F_CPU = 16000000UL --> 16MHz
        // 1MHz = 0.001s
        // one ms has 16000000 / 1000 = 16000 cycles
        count = F_CPU / 1000 / CYCLES_PER_LOOP;
        while (count--)
        {
            // NOP instruction (no operation) takes one cycle in most microprocessors (it's used for synchronization most of the time)
            // 31. Instruction Set Summary (Continued)
            // --> #Clocks 1
            asm volatile ("nop");
        }
    }
}

int main()
{
    // Set the bit 1 of register DDRB to output (1) doesn't change the other bits
    DDRB |= (1 << PORTB1);

    // Set the bit 1 of register PORTB to off (0) doesn't change the other bits
    // Details:
    // 1 --> 00000001
    // PORTB1 = 0x01
    // 1 << 1 --> 00000010
    // ~(00000010) --> 11111101
    // using & 1's from PORTB will stay 1 0 will stay 0 except for bit 1 who will be 0
    PORTB &= ~(1 << PORTB1);

    while(1)
    {
        // inverts the last bit of PORTB (0 -> 1, 1 -> 0)
        PORTB ^= (1 << PORTB1);
        my_delay_ms(500);
    }
}
