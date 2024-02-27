#include "Print.h"

#include <cstdio>
#include <pico/time.h>

PrintUSB stdiousbinst;

size_t Print::write(const char *str, size_t length)
{
    size_t retval = 0;
    for (int i = 0; i < length; i++)
        retval += write(str[i]);
    return retval;
}

size_t Print::print(const char *str)
{
    size_t retval = 0;
    while (*str)
    {
        retval += write(*str);
        str++;
    }
    return retval;
}

size_t Print::print(char c)
{
    return write(c);
}

size_t Print::print(int c, int mode)
{
    static char buffer[34];
    if (mode == DEC)
    {
        snprintf(buffer, sizeof(buffer), "%d", c);
    }
    else if (mode == HEX)
    {
        snprintf(buffer, sizeof(buffer), "%X", c);
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "");
    }
    return print(buffer);
}

size_t Print::print(unsigned long c, int mode)
{
    static char buffer[34];
    if (mode == DEC)
    {
        snprintf(buffer, sizeof(buffer), "%lu", c);
    }
    else if (mode == HEX)
    {
        snprintf(buffer, sizeof(buffer), "%lX", c);
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "");
    }
    return print(buffer);
}

size_t Print::println(const char *str)
{
    return print(str) + print('\n');
}

size_t Print::println(char c)
{
    return print(c) + print('\n');
}

size_t Print::println(int c, int mode)
{
    return print(c, mode) + print('\n');
}

size_t Print::println(unsigned long c, int mode)
{
    return print(c, mode) + print('\n');
}

size_t PrintUSB::print(const char *str)
{
    return printf("%s", str);
}

size_t PrintUSB::print(char c)
{
    return printf("%c", c);
}

size_t PrintUSB::print(int c, int mode)
{
    if (mode == DEC)
    {
        return printf("%d", c);
    }
    else if (mode == HEX)
    {
        return printf("%X", c);
    }
    return 0;
}

size_t PrintUSB::print(unsigned long c, int mode)
{
    if (mode == DEC)
    {
        return printf("%lu", c);
    }
    else if (mode == HEX)
    {
        return printf("%lX", c);
    }
    return 0;
}

size_t PrintUSB::println(const char *str)
{
    return printf("%s\n", str);
}

size_t PrintUSB::println(char c)
{
    return printf("%c\n", c);
}

size_t PrintUSB::println(int c, int mode)
{
    if (mode == DEC)
    {
        return printf("%d\n", c);
    }
    else if (mode == HEX)
    {
        return printf("%X\n", c);
    }
    return 0;
}

size_t PrintUSB::println(unsigned long c, int mode)
{
    if (mode == DEC)
    {
        return printf("%lu\n", c);
    }
    else if (mode == HEX)
    {
        return printf("%lX\n", c);
    }
    return 0;
}

size_t PrintUSB::write(uint8_t c)
{
    return printf("%c", c);
}