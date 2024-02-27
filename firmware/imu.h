#ifndef IMU_H
#define IMU_H

#include <cstdint>
#include "Wire.h"

class IMU
{
public:
    struct Data
    {
        int16_t accel[3];
        int16_t accel_denom;
        int16_t gyro[3];
        int16_t gyro_denom;
        int16_t magn[3];
        int16_t magn_denom;
        int16_t temp;
        int16_t temp_denom;
    };

    IMU(i2c_inst_t *i2c);
    Data read() const;

protected:
    TwoWire &i2c_;
    int16_t accel_denom_;
    int16_t gyro_denom_;
    int16_t magn_denom_;
    int16_t temp_denom_ = 16;
    int16_t offset_g_[3];
    int16_t offset_x_[3];
    int16_t offset_m_[3];
    int16_t offset_temp_ = 25 * temp_denom_;
    uint8_t address_magn_, address_ag_;

    uint8_t readByte(uint8_t address, uint8_t register_address) const;
    int readBytes(uint8_t address, uint8_t register_address, uint8_t *dest, uint8_t length) const;
    void writeByte(uint8_t address, uint8_t register_address, uint8_t byte) const;
};

#endif
