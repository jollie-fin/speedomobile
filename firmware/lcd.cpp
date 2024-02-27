#include "lcd.h"
#include <cstdio>
#include <cstdarg>
#include <pico/time.h>

Display::Box::Box(int col, int line, int length)
    : col_(col), line_(line), length_(length)
{
}

bool Display::Box::changed() const
{
    return memcmp(content_, previous_, length_) != 0;
}

void Display::Box::update(hd44780_I2Cexp &lcd)
{
    if (!changed())
        return;
    lcd.setCursor(col_, line_);
    lcd.write(content_, length_);
    memcpy(previous_, content_, max_content_length_);
}

void Display::Box::set(const char *fmt, ...)
{
    va_list l;
    va_start(l, fmt);
    size_t written = vsnprintf(content_, length_ + 1, fmt, l);
    // printf("set written %zu\n", written);
    memset(content_ + written, ' ', length_ - written);
    index_ = written;
    va_end(l);
}

void Display::Box::append(const char *fmt, ...)
{
    if (index_ >= length_)
        return;
    va_list l;
    va_start(l, fmt);
    size_t written = vsnprintf(content_ + index_, length_ - index_ + 1, fmt, l);
    index_ += written;
    content_[index_] = ' ';
    va_end(l);
}

Display::Display(
    i2c_inst_t *i2c,
    uint8_t address,
    std::initializer_list<Box *> boxes)
    : wire_(i2c), lcd_{wire_, address}
{
    lcd_.begin(20, 4);
    lcd_.clear();
    // lcd_.cursor();
    for (Box *b : boxes)
        addBox(b);
}

bool Display::addBox(Box *b)
{
    if (box_count_ < max_box_count_)
    {
        boxes_[box_count_++] = b;
        return true;
    }
    return false;
}

void Display::update()
{
    for (Box *b : boxes_)
        if (b)
            b->update(lcd_);
}
