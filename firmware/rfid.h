#pragma once

#include "hardware/i2c.h"
#include <PN532.h>
#include <PN532_I2C.h>
#include <Wire.h>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <algorithm>

class RFID
{
public:
    struct uid
    {
        uint8_t data[8] = {};
        uint8_t length = 0;

        uid() = default;
        uid(std::initializer_list<uint8_t> l)
        {
            length = std::min(sizeof(data), l.size());
            std::copy(l.begin(), l.begin() + length, data);
        }
        friend std::strong_ordering operator<=>(const uid &a, const uid &b)
        {
            if (a.length == b.length)
            {
                auto cmp = memcmp(a.data, b.data, a.length);
                if (cmp < 0)
                    return std::strong_ordering::less;
                else if (cmp > 0)
                    return std::strong_ordering::greater;
                else
                    return std::strong_ordering::equivalent;
            }
            else
            {
                return a.length <=> b.length;
            }
        }
        friend bool operator==(const uid &a, const uid &b)
        {
            return a.length == b.length && memcmp(a.data, b.data, a.length) == 0;
        }
    };

    RFID(i2c_inst_t *i2c);
    uint32_t getFirmwareVersion();
    void sleep(bool);
    uid check();

private:
    TwoWire i2c_;
    bool asleep_ = false;
    PN532_I2C pn532_i2c_;
    PN532 pn532_;
};