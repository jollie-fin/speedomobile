#pragma once
#include <cstring>
#include "Print.h"

void delay(unsigned long);
unsigned long millis();
unsigned long long micros();

#define HIGH true
#define LOW false

struct __FlashStringHelper
{
};

struct String
{
};