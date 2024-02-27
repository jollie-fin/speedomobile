#include <cstring>
#include <cstdint>

void f()
{
    uint8_t out[150 * 4 * 8];
    uint8_t in[8][150][3];
    memset(out, 0, sizeof(out));
    for (int led = 0; led < 150; led++)
    {
        for (int bit = 0; bit < 8; bit++)
        {
            out[led * 32 + bit] = 0xFF;
        }
    }
    for (int channel = 0; channel < 8; channel++)
    {
        for (int led = 0; led < 150; led++)
        {
            for (int rgb = 0; rgb < 3; rgb++)
            {
                uint8_t color = in[channel][led][rgb];
                for (int bit = 0; bit < 8; bit++)
                {
                    out[led * 32 + rgb * 8 + 8 + bit] |= 1 << channel;
                }
            }
        }
    }
}
