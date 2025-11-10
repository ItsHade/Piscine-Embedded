#ifndef FUNCTION_H
#define FUNCTION_H

void	adc_init(void);
uint16_t	adc_read(uint8_t ch);
uint16_t read_RV1(void);
uint16_t read_LDR(void);
uint16_t read_NTC(void);
uint16_t  read_Temp(void);
uint8_t get_number_pattern(uint8_t number);
uint8_t    display_RV1(uint8_t setup);
uint8_t    display_LDR(uint8_t setup);
uint8_t   display_NTC(uint8_t setup);
uint8_t    display_temp(uint8_t setup);

#endif // FUNCTION_H
