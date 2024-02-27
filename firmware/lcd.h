#pragma once

#include "Wire.h"
#include "hd44780.h"
#include "hd44780ioClass/hd44780_I2Cexp.h"
#include <initializer_list>

class Display
{
public:
    struct Box
    {
        int col_;
        int line_;
        int length_;

    protected:
        static constexpr size_t max_content_length_ = 20;
        char content_[max_content_length_ + 1] = {};
        char previous_[max_content_length_ + 1] = {};
        size_t index_{};

    public:
        Box(int col, int line, int length);
        void set(const char *fmt, ...);
        void append(const char *fmt, ...);

        bool changed() const;
        void update(hd44780_I2Cexp &lcd);
    };

    template <typename... Ts>
    Display(i2c_inst_t *i2c, uint8_t address, Ts... boxes)
        : Display(i2c, address, {boxes...})
    {
    }

    bool addBox(Box *b);
    void update();

protected:
    Display(i2c_inst_t *i2c, uint8_t address, std::initializer_list<Box *> init);

    TwoWire wire_;
    hd44780_I2Cexp lcd_;
    static constexpr size_t max_box_count_ = 20;
    Box *boxes_[max_box_count_] = {};
    size_t box_count_ = {};
};
