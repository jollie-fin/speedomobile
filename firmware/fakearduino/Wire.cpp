#include "Wire.h"
#include <hardware/i2c.h>
#include <cstring>
#include <cstdio>

TwoWire::TwoWire(i2c_inst_t *i2c) : i2c_(i2c)
{
}

void TwoWire::beginTransmission(Address a)
{
    tx_index_ = 0;
    tx_length_ = 0;
    address_ = a;
}

int TwoWire::endTransmission(bool send_stop)
{
    if (tx_length_ == buff_length_)
        return 1;

    int retval = i2c_write_blocking(i2c_, address_, tx_buffer_, tx_length_, !send_stop);
    if (retval < 0)
        return 4;
    return 0;
}

int TwoWire::requestFrom(Address addr, int quantity, bool send_stop)
{
    // clamp to buffer length
    if (quantity > buff_length_)
    {
        quantity = buff_length_;
    }

    // printf("reading %d from 0x%02X %s: ", quantity, addr, send_stop ? "!" : ">");
    // sleep_ms(1);

    // perform blocking read into buffer
    int read = i2c_read_blocking(i2c_, addr, rx_buffer_, quantity, !send_stop);

    // sleep_ms(1);
    // set rx buffer iterator vars
    rx_index_ = 0;
    rx_length_ = read;

    if (read >= 0)
    {
        // for (int i = 0; i < rx_length_; i++)
        //     printf("%02X ", rx_buffer_[i]);
    }
    else
    {
        read = 0;
    }
    // printf("\n");
    return read;
}

int TwoWire::write(uint8_t a)
{
    return write(&a, 1);
}

int TwoWire::write(const uint8_t *payload, int length)
{
    if (tx_length_ + length > buff_length_)
        length = buff_length_ - tx_index_;
    memcpy(tx_buffer_ + tx_index_, payload, length);
    tx_length_ += length;
    tx_index_ += length;
    return length;
}

int TwoWire::read()
{
    int retval = -1;
    if (rx_index_ < rx_length_)
    {
        retval = rx_buffer_[rx_index_++];
    }
    return retval;
}
