#pragma once

#include <cstdint>
#include <cstddef>

#define DEC 0
#define HEX 1

class Print
{
public:
    size_t print(const char *str);
    size_t print(char c);
    size_t print(unsigned long c, int mode = DEC);
    size_t print(int c, int mode = DEC);
    size_t println(const char *str);
    size_t println(char c);
    size_t println(unsigned long c, int mode = DEC);
    size_t println(int c, int mode = DEC);
    virtual size_t write(const char *str, size_t length);
    virtual size_t write(uint8_t c) = 0;
};

class PrintUSB : Print
{
public:
    size_t print(const char *str);
    size_t print(char c);
    size_t print(unsigned long c, int mode = DEC);
    size_t print(int c, int mode = DEC);
    size_t println(const char *str);
    size_t println(char c);
    size_t println(unsigned long c, int mode = DEC);
    size_t println(int c, int mode = DEC);
    size_t write(uint8_t c);
};

extern PrintUSB stdiousbinst;
#define Serial stdiousbinst
