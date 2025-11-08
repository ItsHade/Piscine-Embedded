#include "main.h"

// AHT20 Datasheet
// Trigger measurement data
// Read temperature and humidity data


void aht20_init(void)
{
    uint8_t status = 0;
    _delay_ms(40); // 5.4 Sensor reading process (wait for 40ms after power-on)


    i2c_start();
    i2c_write((AHT20_ADDR << 1) | I2C_WRITE);
    i2c_write(0x71); // get a byte of status word --> to check if the calibration enable bit of the status word is 1
                     // if not 1, send 0xbe command (for initialization) with 2 data byte: 0x08, 0x00
    i2c_stop();
    _delay_ms(20);

    i2c_start();
    i2c_write((AHT20_ADDR << 1) | I2C_READ);
    status = i2c_read_nack(); 
    i2c_stop();

    // if bit 3 of status word is not 1 (= not enabled)
    if (!(status & 0x08))
    {
        i2c_start();
        i2c_write((AHT20_ADDR << 1) | I2C_WRITE);
        i2c_write(0xBE);
        i2c_write(0x08);
        i2c_write(0x00);
        i2c_stop();
        _delay_ms(10); // send 0xbe command for initialization ... then wait for 10ms (datasheet)
    }
}

void aht20_trigger_measurement(void)
{

    // trigger measurement data
    i2c_start();
    i2c_write((AHT20_ADDR << 1 ) | I2C_WRITE);
    i2c_write(0xAC);
    i2c_write(0x33);
    i2c_write(0x00);
    i2c_stop();
    _delay_ms(80); // wait for 80ms for the measurement to be completed (datasheet)
}

void aht20_read(void)
{
    double average_humidity = 0;
    double average_temperature = 0;
    aht20_trigger_measurement(); 
    uint8_t i = 0;
    while (i++ < 3) // average of 3 last measurements
    {
        // read temperature and humidity data
        i2c_start();
        i2c_write((AHT20_ADDR << 1) | I2C_READ);

        i2c_read(); // status byte
        uint8_t h1 = i2c_read();
        uint8_t h2 = i2c_read();
        uint8_t h3 = i2c_read();
        uint8_t t1 = i2c_read();
        uint8_t t2 = i2c_read();
        i2c_read_nack(); // crc check data byte 

        i2c_stop();

        // raw_hum  0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0
        //                                     ^20                 ^12                 ^4 
        uint32_t raw_hum = ((uint32_t)h1 << 12) | ((uint32_t)h2 << 4) | ((uint32_t)h3 >> 4);
        uint32_t raw_temp = (((uint32_t)(h3 & 0x0F)) << 16) | ((uint32_t)t1 << 8) | t2;

        double humidity = ((double)raw_hum / MAX_20BITS_VALUE_F) * 100.0;
        double temperature = ((double)raw_temp / MAX_20BITS_VALUE_F) * 200.0 - 50.0;
        average_humidity += humidity;
        average_temperature += temperature;
        _delay_ms(2000); // it is recommended to measure data every 2 seconds (avoid measurement frequency too high)
    }

    average_humidity /= 3;
    average_temperature /= 3;

    // Sensor Performance:
    // RH resolution ratio 0.024 --> 0.1
    // T resolution ratio 0.01 --> 0.01 
    char s[10];
    char s1[10];
    putstr_uart("Temperature: ");
    putstr_uart(dtostrf(average_temperature, 4, 2, s));
    putstr_uart("C, Humidity: ");
    if (average_humidity == 100.0)
        putstr_uart(dtostrf(average_humidity, 5, 1, s1));
    else
        putstr_uart(dtostrf(average_humidity, 4, 1, s1));
    putstr_uart("%\r\n");
}

// Both humidity and temperature are 20bit values given on 4bytes | 2 pow 20 = 1048576
// 6.1 Relative humidity transformation:
// Humidity[%] = (SRH / 2 pow 20) * 100%
// where SRH is the relative humidity signal read from SDA

// 6.2 Temperature transformation
// T(C) = (ST / 2 pow 20) * 200 - 50
// where ST is the temperature signal read from SDA
int main(void)
{
    init_uart();
    i2c_init();
    aht20_init();

    while (1)
    {
        aht20_read();
    }
}
