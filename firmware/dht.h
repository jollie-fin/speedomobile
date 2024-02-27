#pragma once
#include <stdint.h>

class DHT
{
public:
    DHT(unsigned int pin);

    struct data
    {
        float temperature_;
        float humidity_;
    };

    data read() const;

protected:
    bool wait_for_value(unsigned long timeout_us, bool value) const;
    bool wait_for_0(unsigned long timeout_us) const;
    bool wait_for_1(unsigned long timeout_us) const;
    int readbit() const;
    void pulse(unsigned long width_us) const;
    bool readraw(uint8_t *dest, int max_length) const;

    unsigned int pin_;
};