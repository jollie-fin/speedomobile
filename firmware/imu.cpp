#include "imu.h"
#include "imu_registers.h"

IMU::Data IMU::read() const
{
    Data retval = {.accel_denom = accel_denom_, .gyro_denom = gyro_denom_, .magn_denom = magn_denom_};
    uint8_t temp[6]; // We'll read six bytes from the gyro into temp

    struct Location
    {
        uint8_t address;
        uint8_t location;
        int16_t *destination;
        const int16_t *offset;
        int length;
    };

    Location locations[] = {
        {address_ag_, OUT_X_L_G, retval.gyro, offset_g_, 3},
        {address_ag_, OUT_X_L_XL, retval.accel, offset_x_, 3},
        {address_magn_, OUT_X_L_M, retval.magn, offset_m_, 3},
        {address_ag_, OUT_TEMP_L, &retval.temp, &offset_temp_, 1}};

    for (const auto &[address, location, destination, offset, length] : locations)
    {
        if (readBytes(address, location, temp, length * 2) == length * 2)
        {
            for (int i = 0; i < length; i++)
            {
                destination[i] = (temp[i * 2 + 1] << 8) | temp[i * 2];
                destination[i] -= offset[i];
            }
        }
    }

    return retval;
}

uint8_t IMU::readByte(uint8_t address, uint8_t register_address) const
{
    uint8_t retval;
    i2c_.beginTransmission(address);
    i2c_.write(register_address);
    i2c_.endTransmission(false);
    i2c_.requestFrom(address, 1);

    retval = i2c_.read();
    return retval;
}

int IMU::readBytes(uint8_t address, uint8_t register_address, uint8_t *dest, uint8_t length) const
{
    i2c_.beginTransmission(address);
    i2c_.write(register_address | 0x80);
    i2c_.endTransmission(false);
    return i2c_.requestFrom(address, length);
}

void IMU::writeByte(uint8_t address, uint8_t register_address, uint8_t byte) const
{
    i2c_.beginTransmission(address);
    i2c_.write(register_address);
    i2c_.write(byte);
    i2c_.endTransmission();
}
