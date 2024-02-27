#include "rfid.h"

RFID::RFID(i2c_inst_t *i2c)
    : i2c_(i2c), pn532_i2c_(i2c_), pn532_(pn532_i2c_)
{
    pn532_.begin();
    sleep_ms(1);
    pn532_.SAMConfig();
    sleep_ms(1);
}

RFID::uid RFID::check()
{
    uid retval = {};
    bool success = pn532_.readPassiveTargetID(
        PN532_MIFARE_ISO14443A,
        retval.data,
        &retval.length, 50);
    if (!success)
    {
        retval.length = 0;
    }
    sleep_ms(1);
    return retval;
}

uint32_t RFID::getFirmwareVersion()
{
    return pn532_.getFirmwareVersion();
}
