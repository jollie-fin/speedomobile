#pragma once

#include <hardware/i2c.h>
#include <cstdint>

class TwoWire
{
public:
    using Address = uint8_t;

    TwoWire(i2c_inst_t *);
    int write(uint8_t);
    int write(const uint8_t *payload, int length);
    auto send(auto d) { return write(d); }
    int read();
    auto receive() { return read(); }
    void begin() {}
    void beginTransmission(Address address);
    int endTransmission(bool send_stop = true);
    int requestFrom(Address, int quantity, bool send_stop = true);

private:
    i2c_inst_t *i2c_;

    constexpr static int buff_length_ = 512;
    uint8_t rx_buffer_[buff_length_];
    uint8_t tx_buffer_[buff_length_];
    int rx_index_;
    int tx_index_;
    int rx_length_;
    int tx_length_;
    Address address_;
};

// extern TwoWire Wire;
