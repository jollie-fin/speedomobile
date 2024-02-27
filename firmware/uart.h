#ifndef UART_H
#define UART_H

#include <cstdint>
#include <cstdlib>
#include <variant>
#include "common.h"
#include "pico/multicore.h"

class Uart
{
public:
    Uart();

    struct Request
    {
        uint16_t reg_;
        size_t length_;
        bool shall_read_;
        uint8_t payload_[16];
    };

    struct Samplerate
    {
        char which_;
        uint32_t samplerate_;
    };

    struct None
    {
    };

    std::variant<None, Request, Samplerate> read();
    void write(const Request &r);
    void writeData(char channel, const vect &v, int64_t timestamp);
    void writeData(char channel, int16_t v, int64_t timestamp);

private:
    void send(const uint8_t *data, uint8_t length);
    mutex_t mutex_;
};

#endif
