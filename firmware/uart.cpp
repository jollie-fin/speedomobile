#include <cstdio>
#include "pico/stdio.h"
#include <string.h>
#include "uart.h"

static uint8_t magic_key[] = {0xDE, 0xAD, 0xBE, 0xEF};
uint32_t nb_missed_bytes = 0;

static void wait_for_magic()
{
    for (int i = 0; i < sizeof(magic_key); i++)
    {
        if (magic_key[i] != getchar())
        {
            i = -1;
            nb_missed_bytes++;
        }
    }
}

Uart::Uart()
{
    mutex_init(&mutex_);
}

char encoding_table[] = "0123456789ABCDEF";
// void w(const uint8_t *data, uint8_t length, uint8_t l2, FILE *f)
// {
//     uint8_t payload[length * l2 * 2];
//     for (int i = 0; i < length * l2; i++)
//     {
//         payload[i * 2] = encoding_table[data[i] & 0xF];
//         payload[i * 2 + 1] = encoding_table[data[i] >> 4];
//     }
//     fwrite(payload, sizeof(payload), 1, stdout);
// }

void concat(uint8_t *&dst, const uint8_t *src, uint8_t length, int &new_length)
{
    memcpy(dst, src, length);
    dst += length;
    new_length += length;
}
template <size_t N>
void concat(uint8_t *&dst, uint8_t (&data)[N], int &new_length)
{
    concat(dst, data, N, new_length);
}
void concat(uint8_t *&dst, uint8_t src, int &new_length)
{
    concat(dst, &src, 1, new_length);
}
// void Uart::send(const uint8_t *data, uint8_t length)
// {
//     int bytes_count = 0;
//     uint8_t payload[sizeof(magic_key) + 1 + length * 2 + 1];
//     uint8_t *dst = payload;

//     mutex_enter_blocking(&mutex_);
//     concat(dst, magic_key, bytes_count);
//     uint8_t new_length = length * 2 + ' ';
//     concat(dst, new_length, bytes_count);
//     for (int i = 0; i < length; i++)
//     {
//         concat(dst, encoding_table[data[i] >> 4], bytes_count);
//         concat(dst, encoding_table[data[i] & 0xF], bytes_count);
//     }
//     concat(dst, '\n', bytes_count);
//     // concat(dst, '\0', bytes_count);

//     fwrite(payload, bytes_count, 1, stdout);

//     //  fflush(stdout);
//     mutex_exit(&mutex_);
// }

void Uart::send(const uint8_t *data, uint8_t length)
{
    int bytes_count = 0;
    uint8_t payload[sizeof(magic_key) + 1 + length * 2 + 1];
    uint8_t *dst = payload;

    // mutex_enter_blocking(&mutex_);
    // auto start = get_absolute_time();

    memcpy(dst, magic_key, sizeof(magic_key));
    dst += sizeof(magic_key);
    // concat(dst, magic_key, bytes_count);

    // concat(dst, ' ', bytes_count);
    auto new_length_location = dst;
    *dst++ = ' ';
    auto bytes_count_after_length = dst - payload;
    for (int i = 0; i < length; i++)
    {
        // printf("%02X ", data[i]);
        if (data[i] < 32 || data[i] == '#' || data[i] == 0x7F)
        {
            *dst++ = '#';
            *dst++ = data[i] + '#';
            // printf("special char #%d\n", (int)data[i]);
            // concat(dst, '#', bytes_count);
            // concat(dst, data[i] + '#', bytes_count);
            // printf("special chars %c %c\n", dst[-2], dst[-1]);
        }
        else
        {
            *dst++ = data[i];
            // concat(dst, data[i], bytes_count);
        }
    }
    // printf("\n");
    *new_length_location = dst - payload - bytes_count_after_length + ' ';
    // concat(dst, '\n', bytes_count);
    *dst++ = '\n';
    // for (int i = 5; i < bytes_count; i++)
    //     printf("%02X ", payload[i]);
    // printf("\n");

    // concat(dst, '\0', bytes_count);
    // auto middle = get_absolute_time();
    fwrite(payload, dst - payload, 1, stdout);
    // auto end = get_absolute_time();
    // printf("Send %lld %lld\n", absolute_time_diff_us(start, middle), absolute_time_diff_us(middle, end));
    //  fflush(stdout);
    // mutex_exit(&mutex_);
}

std::variant<Uart::None, Uart::Request, Uart::Samplerate> Uart::read()
{
    wait_for_magic();
    size_t length = getchar();
    if (length > 20 || length <= 1)
        return {};
    int which = getchar();

    length -= 1;
    static uint8_t payload[20];
    fread(payload, length, 1, stdin);

    switch (payload[0])
    {
    case 'R':
    {
        if (length != 3)
            return {};
        uint16_t reg;
        memcpy(&reg, payload, sizeof(reg));
        return Request{reg, payload[2], true};
    }
    case 'W':
    {
        if (length <= 3)
            return {};
        uint16_t reg;
        memcpy(&reg, payload, sizeof(reg));
        Request retval{reg, length - 2, true};
        memcpy(retval.payload_, payload + 2, retval.length_);
        return retval;
    }
    case 'S':
    {
        if (length != 5)
            return {};
        uint32_t sr;
        uint8_t channel = payload[0];
        memcpy(&sr, payload + 1, sizeof(sr));
        return Samplerate{(char)channel,
                          sr};
    }
    default:
        return {};
    }
}

void Uart::write(const Uart::Request &r)
{
    auto length = (1 + r.length_ + sizeof(r.reg_));
    if (length > 30)
        length = 30;

    uint8_t payload[30] = {
        r.shall_read_ ? 'R' : 'W'};
    memcpy(payload + 1, &r.reg_, 2);
    memcpy(payload + 1 + sizeof(r.reg_), r.payload_, r.length_);
    send(payload, length);
}

void Uart::writeData(char channel, const vect &v, int64_t timestamp)
{
    constexpr auto length = 2 + sizeof(timestamp) + sizeof(v.payload);
    uint8_t payload[length * 4] = {
        'S',
        channel};
    memcpy(payload + 2, &timestamp, sizeof(timestamp));
    memcpy(payload + 2 + sizeof(timestamp), v.payload, sizeof(v.payload));
    send(payload, length);
}

void Uart::writeData(char channel, int16_t v, int64_t timestamp)
{
    constexpr auto length = 2 + sizeof(timestamp) + sizeof(v);

    uint8_t payload[length * 4] = {
        'S',
        channel};
    memcpy(payload + 2, &timestamp, sizeof(timestamp));
    memcpy(payload + 2 + sizeof(timestamp), &v, sizeof(v));
    send(payload, length);
}
