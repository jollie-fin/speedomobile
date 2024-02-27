#include "dht.h"
#include "pico/time.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdint.h>

DHT::DHT(unsigned int pin) : pin_(pin)
{
    gpio_init(pin_);
    gpio_set_dir(pin_, GPIO_IN);
    gpio_put(pin_, 0);
}

bool DHT::wait_for_value(unsigned long timeout_us, bool value) const
{
    auto start = get_absolute_time();
    while (1)
    {
        auto current = get_absolute_time();
        auto elapsed = absolute_time_diff_us(start, current);
        if (elapsed > timeout_us)
        {
            // printf("Waited %lu us for %d got nothing\n", timeout_us, value);
            return false;
        }
        if (gpio_get(pin_) == value)
            return true;
    }
}

bool DHT::wait_for_1(unsigned long timeout_us) const
{
    return wait_for_value(timeout_us, 1);
}

bool DHT::wait_for_0(unsigned long timeout_us) const
{
    return wait_for_value(timeout_us, 0);
}

void DHT::pulse(unsigned long width_us) const
{
    gpio_set_dir(pin_, GPIO_OUT);
    sleep_us(width_us);
    gpio_set_dir(pin_, GPIO_IN);
}

int DHT::readbit() const
{
    if (!wait_for_1(70))
        return -1;
    if (wait_for_0(40))
        return 0;
    else if (wait_for_0(40))
        return 1;
    else
        return -2;
}

bool DHT::readraw(uint8_t *dest, int max_length) const
{
    pulse(18 * 1000);
    if (!wait_for_0(50))
    {
        printf("first 0 missing\n");
        return false;
    }
    if (!wait_for_1(100))
    {
        printf("first 1 missing\n");
        return false;
    }
    if (!wait_for_0(100))
    {
        printf("second 0 missing\n");
        return false;
    }

    for (int i = 0; i < max_length; i++)
    {
        dest[i] = 0;
        for (int j = 7; j >= 0; j--)
        {
            auto bit = readbit();
            if (bit >= 0)
                dest[i] |= bit << j;
            else
            {
                printf("bit error %d at %d*8+%d\n", bit, i, j);
                return false;
            }
        }
    }
    return true;
}

DHT::data DHT::read() const
{
    uint8_t buffer[5] = {};
    if (!readraw(buffer, sizeof(buffer)))
    {
        printf("Invalid read\n");
        return {};
    }
    uint8_t checksum = 0;
    for (int i = 0; i < 4; i++)
        checksum += buffer[i];
    if (checksum != buffer[4])
    {
        printf("Invalid checksum\n");
        return {};
    }
    // for (int i = 0; i < 4; i++)
    //     printf("%02X ", buffer[i]);
    // printf("\n");
    uint16_t humidity_16 = buffer[0];
    humidity_16 <<= 8;
    humidity_16 |= buffer[1];
    float humidity = humidity_16 / 10.;
    uint8_t sign_mask = 1 << 7;
    bool is_negative = buffer[2] & sign_mask;
    int16_t temperature_16 = buffer[2] & ~sign_mask;
    temperature_16 <<= 8;
    temperature_16 |= buffer[3];
    if (is_negative)
        temperature_16 *= -1;
    float temperature = temperature_16 / 10.;
    printf("%.1f %.1f\n", humidity, temperature);
    return {};
}
