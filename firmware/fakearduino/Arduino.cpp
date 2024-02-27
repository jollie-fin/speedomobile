#include "Arduino.h"
#include "pico/time.h"

static auto init_timestamp = get_absolute_time();

void delay(unsigned long d)
{
    sleep_ms(d);
}

unsigned long millis()
{
    return absolute_time_diff_us(init_timestamp,
                                 get_absolute_time()) /
           1000;
}

unsigned long long micros()
{
    return absolute_time_diff_us(init_timestamp,
                                 get_absolute_time());
}
