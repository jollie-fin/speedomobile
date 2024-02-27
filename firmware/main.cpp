#include "pico/stdio.h"
#include <cstdio>
// #include "hardware/spi.h"
#include "hardware/i2c.h"

#include "pico/binary_info.h"
#include "pico/stdlib.h"
// #include "pico/mutex.h"
#include "pico/time.h"
// #include "pico/util/queue.h"
// #include "pico/multicore.h"
#include <cstdint>
// #include "imu.h"
// #include "uart.h"
// #include "imu20948def.h"
#include "pico/util/queue.h"
#include "pico/cyw43_arch.h"
#include "dht.h"

#include "rfid.h"
#include "lcd.h"

// class Mainloop
// {
// public:
//     struct VectData
//     {
//         vect v;
//         int64_t timestamp;
//     };
//     struct ScalData
//     {
//         int16_t v;
//         int64_t timestamp;
//     };
//     Mainloop() : imu_(spi_default, i2c_default, PICO_DEFAULT_SPI_CSN_PIN)
//     {
//         self = this;
//         init_timestamp_ = get_absolute_time();
//         printf("Mainloop\n");
//         // multicore_launch_core1(dummy);
//         queue_init(&accel_sr_, sizeof(uint32_t), 2);
//         queue_init(&gyro_sr_, sizeof(uint32_t), 2);

//         queue_init(&accel_vect_, sizeof(VectData), 360);
//         queue_init(&gyro_vect_, sizeof(VectData), 720);
//         queue_init(&magn_vect_, sizeof(VectData), 8);
//         queue_init(&temp_data_, sizeof(ScalData), 8);

//         // mainloop1();
//         multicore_launch_core1(mainloop1);

//         while (1)
//         {
//             handleuart();
//             handletimer();
//         }
//     }

// private:
//     int64_t current_us()
//     {
//         return absolute_time_diff_us(init_timestamp_,
//                                      get_absolute_time());
//     }

//     void handleuart()
//     {
//         VectData vd;
//         ScalData sd;
//         while (queue_try_remove(&accel_vect_, &vd))
//         {
//             uart_.writeData('A', vd.v, vd.timestamp);
//         }
//         while (queue_try_remove(&gyro_vect_, &vd))
//         {
//             uart_.writeData('G', vd.v, vd.timestamp);
//         }
//         while (queue_try_remove(&magn_vect_, &vd))
//         {
//             uart_.writeData('M', vd.v, vd.timestamp);
//         }
//         while (queue_try_remove(&temp_data_, &sd))
//         {
//             uart_.writeData('T', sd.v, sd.timestamp);
//         }
//         // auto reading = uart_.read();
//         // if (auto *rw_req = std::get_if<Uart::Request>(&reading);
//         //     rw_req)
//         // {
//         //     if (rw_req->shall_read_)
//         //     {
//         //         imu_.read(rw_req->reg_, rw_req->payload_, rw_req->length_);
//         //         uart_.write(*rw_req);
//         //     }
//         //     else
//         //     {
//         //         imu_.write(rw_req->reg_, rw_req->payload_, rw_req->length_);
//         //     }
//         // }
//         // if (auto *samplerate = std::get_if<Uart::Samplerate>(&reading);
//         //     samplerate)
//         // {
//         //     switch (samplerate->which_)
//         //     {
//         //     case 'A':
//         //         queue_try_add(&accel_sr_, &samplerate->samplerate_);
//         //         break;
//         //     case 'G':
//         //         queue_try_add(&gyro_sr_, &samplerate->samplerate_);
//         //         break;
//         //     case 'M':
//         //         magn_delay_us_ = 1000000 / samplerate->samplerate_;
//         //         break;
//         //     case 'T':
//         //         temp_delay_us_ = 1000000 / samplerate->samplerate_;
//         //         break;
//         //     default:
//         //         break;
//         //     }
//         // }
//     }

//     void handletimer()
//     {
//         // printf("Magn\n");
//         // static auto last = get_absolute_time();
//         auto current = current_us();
//         if (current > last_magn_ + magn_delay_us_)
//         {
//             imu_.enableMagn(1);
//             last_magn_ += magn_delay_us_;
//             next_magn_ = current + next_magn_delay_us_;
//         }
//         // // if (imu_.hasMagn() || current > last_magn_ + magn_delay_us_)
//         if (current > next_magn_)
//         {
//             // auto now = get_absolute_time();
//             // printf("elapsed %lld\n", absolute_time_diff_us(last, now));
//             VectData vd;
//             vd.timestamp = current_us();
//             // imu_.hasMagn();

//             vd.v = imu_.readMagn();
//             // imu_.enableMagn(1);
//             queue_try_add(&magn_vect_, &vd);
//             // return true;
//             // last_magn_ = current;
//             // last_magn_ += magn_delay_us_;
//             // last = get_absolute_time();
//         }
//         if (current > last_temp_ + temp_delay_us_)
//         {
//             //     auto temp = imu_.readTemp();
//             //     //            uart_.writeData('T', temp, current);
//             // }
//             // }
//             // static bool readTemp(repeating_timer_t *rt)
//             // {
//             ScalData sd;
//             sd.timestamp = current_us();
//             sd.v = imu_.readTemp();
//             queue_try_add(&temp_data_, &sd);
//             last_temp_ += temp_delay_us_;
//             // return true;
//         }
//     }

//     static void mainloop1()
//     {
//         uint32_t accel_sr = 4500;
//         uint32_t gyro_sr = 9000;
//         uint32_t accel_delay_us = 1000000 / accel_sr;
//         uint32_t gyro_delay_us = 1000000 / gyro_sr;
//         int64_t last_accel = 0;
//         int64_t last_gyro = 0;

//         while (1)
//         {
//             if (queue_try_remove(&self->accel_sr_, &accel_sr))
//             {
//                 accel_delay_us = 1000000 / accel_sr;
//             }
//             if (queue_try_remove(&self->gyro_sr_, &gyro_sr))
//             {
//                 gyro_delay_us = 1000000 / gyro_sr;
//             }
//             auto current = self->current_us();
//             if (current > last_accel + accel_delay_us)
//             {
//                 VectData vd;
//                 vd.timestamp = self->current_us();
//                 vd.v = self->imu_.readAccel();
//                 queue_try_add(&self->accel_vect_, &vd);
//                 // self->uart_.writeData('A', accel, current);
//                 last_accel += accel_delay_us;
//             }
//             if (current > last_gyro + gyro_delay_us)
//             {
//                 VectData vd;
//                 vd.timestamp = self->current_us();
//                 vd.v = self->imu_.readGyro();
//                 queue_try_add(&self->gyro_vect_, &vd);
//                 // self->uart_.writeData('G', gyro, current);
//                 last_gyro += gyro_delay_us;
//             }
//         }
//     }

// protected:
//     IMU imu_;
//     Uart uart_;
//     queue_t accel_sr_;
//     queue_t gyro_sr_;

//     queue_t accel_vect_;
//     queue_t gyro_vect_;
//     queue_t magn_vect_;
//     queue_t temp_data_;

//     uint32_t magn_delay_us_ = 11000;
//     uint32_t next_magn_delay_us_ = 10000;
//     uint32_t temp_delay_us_ = 1000000;
//     int64_t next_magn_ = 0;
//     int64_t last_magn_ = 0;
//     int64_t last_temp_ = 0;
//     absolute_time_t init_timestamp_;
//     static Mainloop *self;
// };

// Mainloop *Mainloop::self = nullptr;

bool reserved_addr(uint8_t addr)
{
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

int main()
{
    stdio_usb_init();

    if (cyw43_arch_init())
    {
        printf("Wi-Fi init failed");
        return -1;
    }
    // DHT dht{13};
    // while (true)
    // {
    //     cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    //     sleep_ms(250);
    //     cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    //     dht.read();
    //     sleep_ms(2000);
    // }

    gpio_set_function(14, GPIO_FUNC_I2C);
    gpio_set_function(15, GPIO_FUNC_I2C);
    i2c_init(i2c1, 100 * 1000);

    // while (true)
    // {
    //     cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    //     sleep_ms(250);
    //     cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    //     sleep_ms(250);
    // }

    for (int i = 0; i < 3; i++)
    {
        printf("Wait for it... %d\n", i);
        sleep_ms(1000);
    }

    if (1)
    {
        for (int addr = 0; addr < (1 << 7); ++addr)
        {
            if (addr % 16 == 0)
            {
                printf("%02x ", addr);
            }

            // Perform a 1-byte dummy read from the probe address. If a slave
            // acknowledges this address, the function returns the number of bytes
            // transferred. If the address byte is ignored, the function returns
            // -1.

            // Skip over any reserved addresses.
            int ret;
            uint8_t rxdata;
            if (reserved_addr(addr))
                ret = PICO_ERROR_GENERIC;
            else
                ret = i2c_read_blocking(i2c1, addr, &rxdata, 1, false);

            printf(ret < 0 ? "." : "@");
            printf(addr % 16 == 15 ? "\n" : "  ");
            // sleep_us(1000);
        }
        sleep_us(500000);
    }

    Display::Box rfid_box{0, 0, 14};
    Display lcd{i2c1, 0x3F, &rfid_box};

    RFID rfid{i2c1};

    uint32_t versiondata = rfid.getFirmwareVersion();
    if (!versiondata)
    {
        printf("Didn't find PN53x board");
    }
    else
    {
        printf("version %lu\n", versiondata);
        // auto uid = rfid.check();
        // while (1)
        //     ;
    }

    RFID::uid previous{};
    RFID::uid reference{0xE3, 0x81, 0x95, 0x35};
    while (1)
    {
        auto uid = rfid.check();
        rfid_box.set("");

        if (uid != previous)
        {
            if (uid == reference)
            {
                printf("Matched !");
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
            }
            else
            {
                printf("no match\n");
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
            }
            if (uid.length != 0)
            {
                for (int i = 0; i < uid.length; i++)
                    rfid_box.append("%02X", uid.data[i]);
                for (int i = 0; i < uid.length; i++)
                    printf("0x%02X ", uid.data[i]);
                printf("\n");
            }
            else
            {
                printf("None\n");
            }
            lcd.update();
        }
        previous = uid;
        sleep_ms(1);
    }

    while (true)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);
    }

    // spi_init(spi_default, 2000 * 1000);
    // spi_set_format(spi_default, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
    // uint8_t dummy = 0;
    // spi_write_blocking(spi_default, &dummy, 1);
    // gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    // gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    // gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    // gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    // gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    // gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    // for (int i = 0; i < 3; i++)
    // {
    //     printf("Wait for it... %d\n", i);
    //     sleep_ms(1000);
    // }
    // Mainloop{};
    return 0;
}
