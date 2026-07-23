/*
 * SPDX-FileCopyrightText: 2026 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "unit_8servos2.hpp"

bool M5Unit8Servos2::writeBytes(uint8_t reg, const uint8_t *buffer, uint8_t length)
{
    if (_wire == nullptr || (buffer == nullptr && length != 0)) return false;

    _wire->beginTransmission(_addr);
    _wire->write(reg);
    if (length > 0) {
        _wire->write(buffer, length);
    }
    return (_wire->endTransmission() == 0);
}

bool M5Unit8Servos2::readBytes(uint8_t reg, uint8_t *buffer, uint8_t length)
{
    if (_wire == nullptr || buffer == nullptr || length == 0) return false;

    _wire->beginTransmission(_addr);
    _wire->write(reg);
    if (_wire->endTransmission(false) != 0) {
        return false;
    }

    // Check if request was successful
    if (_wire->requestFrom(_addr, length) != length) {
        return false;
    }

    for (int i = 0; i < length; i++) {
        buffer[i] = _wire->read();
    }
    return true;
}

bool M5Unit8Servos2::writeByte(uint8_t reg, uint8_t data)
{
    return writeBytes(reg, &data, 1);
}

uint8_t M5Unit8Servos2::readByte(uint8_t reg)
{
    uint8_t data = 0;
    readBytes(reg, &data, 1);
    return data;
}

bool M5Unit8Servos2::begin(TwoWire *wire, uint8_t addr, int sda, int scl, uint32_t speed)
{
    if (wire == nullptr) return false;

    _wire = wire;
    _addr = addr;

    // Only initialize pins if valid pins are provided
    // 仅在提供有效引脚时初始化
    if (sda != -1 && scl != -1) {
        _wire->begin((int)sda, (int)scl);
    }

    _wire->setClock(speed);

    // Check communication
    _wire->beginTransmission(_addr);
    uint8_t error = _wire->endTransmission();

    if (error == 0) {
        serialPrintln("Unit 8Servo2 found");
        return true;
    } else {
        serialPrintf("Unit 8Servo2 error: %d\n", error);
        return false;
    }
}

void M5Unit8Servos2::setMode(uint8_t channel, m5_8servo2_mode_t mode)
{
    if (channel > 7) return;
    writeByte(UNIT_8SERVOS2_REG_MODE_START + channel, (uint8_t)mode);
}

m5_8servo2_mode_t M5Unit8Servos2::getMode(uint8_t channel)
{
    if (channel > 7) return M5_8SERVOS2_MODE_INPUT;
    uint8_t val = readByte(UNIT_8SERVOS2_REG_MODE_START + channel);
    return (m5_8servo2_mode_t)val;
}

void M5Unit8Servos2::setInputPull(uint8_t channel, m5_8servo2_pull_t pull)
{
    if (channel > 7) return;
    writeByte(UNIT_8SERVOS2_REG_INPUT_PULL_START + channel, (uint8_t)pull);
}

m5_8servo2_pull_t M5Unit8Servos2::getInputPull(uint8_t channel)
{
    if (channel > 7) return M5_8SERVOS2_PULL_NONE;
    uint8_t val = readByte(UNIT_8SERVOS2_REG_INPUT_PULL_START + channel);
    return (m5_8servo2_pull_t)val;
}

bool M5Unit8Servos2::getDigitalInput(uint8_t channel)
{
    if (channel > 7) return false;
    uint8_t val = readByte(UNIT_8SERVOS2_REG_INPUT_STATUS_START + channel);
    return (val == 1);
}

void M5Unit8Servos2::setDigitalOutput(uint8_t channel, bool level)
{
    if (channel > 7) return;
    writeByte(UNIT_8SERVOS2_REG_OUTPUT_STATUS_START + channel, level ? 1 : 0);
}

bool M5Unit8Servos2::getDigitalOutput(uint8_t channel)
{
    if (channel > 7) return false;
    uint8_t val = readByte(UNIT_8SERVOS2_REG_OUTPUT_STATUS_START + channel);
    return (val == 1);
}

uint16_t M5Unit8Servos2::getADCRaw(uint8_t channel)
{
    if (channel > 7) return 0;
    uint8_t buf[2] = {0};
    readBytes(UNIT_8SERVOS2_REG_ADC_START + (channel * 2), buf, 2);
    return (uint16_t)(buf[0] | (buf[1] << 8));
}

uint16_t M5Unit8Servos2::getVoltageMV(uint8_t channel)
{
    uint32_t raw  = getADCRaw(channel);
    uint32_t vref = getSysVREF();
    // Avoid floating point math for speed on small MCUs
    // 避免浮点运算以提高性能
    // V = (Raw / 4095) * VREF
    if (vref == 0) return 0;
    return (uint16_t)((raw * vref) / 4095);
}

void M5Unit8Servos2::setServoAngle(uint8_t channel, uint8_t angle)
{
    if (channel > 7) return;
    if (angle > 180) angle = 180;
    writeByte(UNIT_8SERVOS2_REG_SERVO_START + channel, angle);
}

uint8_t M5Unit8Servos2::getServoAngle(uint8_t channel)
{
    if (channel > 7) return 0;
    return readByte(UNIT_8SERVOS2_REG_SERVO_START + channel);
}

void M5Unit8Servos2::setRGBConfig(uint8_t channel, uint8_t num_leds, bool refresh)
{
    if (channel > 7) return;
    if (num_leds > 16) num_leds = 16;
    // Register: [5 bits count] [1 bit refresh] [2 bits rsvd]
    uint8_t config_val = (num_leds & 0x1F) | (refresh ? 0x20 : 0x00);
    writeByte(UNIT_8SERVOS2_REG_RGB_CONF_START + channel, config_val);
}

uint8_t M5Unit8Servos2::getRGBConfig(uint8_t channel)
{
    if (channel > 7) return 0;
    return readByte(UNIT_8SERVOS2_REG_RGB_CONF_START + channel);
}

void M5Unit8Servos2::setRGBBufferIndex(uint8_t buffer_index, uint32_t color)
{
    if (buffer_index > 15) return;

    uint32_t value = color;

    writeBytes(UNIT_8SERVOS2_REG_RGB_BUFFER_START + (buffer_index * 4), (uint8_t *)&value, 4);
}

uint32_t M5Unit8Servos2::getRGBBufferIndex(uint8_t buffer_index)
{
    if (buffer_index > 15) return 0;

    uint32_t value = 0;

    readBytes(UNIT_8SERVOS2_REG_RGB_BUFFER_START + (buffer_index * 4), (uint8_t *)&value, 4);

    return value;
}

void M5Unit8Servos2::setRGBBuffer(const uint32_t *colors, uint8_t num_leds)
{
    if (colors == nullptr || num_leds == 0) return;
    if (num_leds > 16) num_leds = 16;
    writeBytes(UNIT_8SERVOS2_REG_RGB_BUFFER_START, (const uint8_t *)colors, num_leds * 4);
}

void M5Unit8Servos2::getRGBBuffer(uint32_t *colors, uint8_t num_leds)
{
    if (colors == nullptr || num_leds == 0) return;
    if (num_leds > 16) num_leds = 16;
    readBytes(UNIT_8SERVOS2_REG_RGB_BUFFER_START, (uint8_t *)colors, num_leds * 4);
}

void M5Unit8Servos2::setPWMDuty(uint8_t channel, uint8_t duty)
{
    if (channel > 7) return;
    if (duty > 100) duty = 100;
    writeByte(UNIT_8SERVOS2_REG_PWM_DUTY_START + channel, duty);
}

uint8_t M5Unit8Servos2::getPWMDuty(uint8_t channel)
{
    if (channel > 7) return 0;
    return readByte(UNIT_8SERVOS2_REG_PWM_DUTY_START + channel);
}

void M5Unit8Servos2::setTimerFrequency(uint8_t timer_group, uint16_t freq)
{
    if (timer_group > 1) return;
    uint8_t reg = UNIT_8SERVOS2_REG_TIM_FREQ_START + (timer_group * 2);
    uint8_t buf[2];
    buf[0] = (uint8_t)(freq & 0xFF);
    buf[1] = (uint8_t)((freq >> 8) & 0xFF);
    writeBytes(reg, buf, 2);
}

uint16_t M5Unit8Servos2::getTimerFrequency(uint8_t timer_group)
{
    if (timer_group > 1) return 0;
    uint8_t reg    = UNIT_8SERVOS2_REG_TIM_FREQ_START + (timer_group * 2);
    uint8_t buf[2] = {0};
    readBytes(reg, buf, 2);
    return (uint16_t)(buf[0] | (buf[1] << 8));
}

void M5Unit8Servos2::getUID(uint8_t uid[], uint8_t length)
{
    if (uid == nullptr || length == 0) return;
    if (length > UNIT_8SERVOS2_UID_LENGTH) length = UNIT_8SERVOS2_UID_LENGTH;
    readBytes(UNIT_8SERVOS2_REG_UID_START, uid, length);
}

uint16_t M5Unit8Servos2::getSysVREF()
{
    uint8_t buf[2] = {0};
    readBytes(UNIT_8SERVOS2_REG_SYS_ANALOG_START, buf, 2);
    return (uint16_t)(buf[0] | (buf[1] << 8));
}

uint16_t M5Unit8Servos2::getGroveVoltage()
{
    uint8_t buf[2] = {0};
    readBytes(UNIT_8SERVOS2_REG_SYS_ANALOG_START + 2, buf, 2);
    return (uint16_t)(buf[0] | (buf[1] << 8));
}

uint16_t M5Unit8Servos2::getDCVoltage()
{
    uint8_t buf[2] = {0};
    readBytes(UNIT_8SERVOS2_REG_SYS_ANALOG_START + 4, buf, 2);
    return (uint16_t)(buf[0] | (buf[1] << 8));
}

uint16_t M5Unit8Servos2::getSysCurrent()
{
    uint8_t buf[2] = {0};
    readBytes(UNIT_8SERVOS2_REG_SYS_ANALOG_START + 6, buf, 2);
    return (uint16_t)(buf[0] | (buf[1] << 8));
}

uint8_t M5Unit8Servos2::getFirmwareVersion()
{
    return readByte(UNIT_8SERVOS2_REG_VERSION);
}

uint8_t M5Unit8Servos2::getI2CAddressOffset()
{
    return readByte(UNIT_8SERVOS2_REG_ADDR_OFFSET);
}

bool M5Unit8Servos2::refreshI2CAddress()
{
    uint8_t offset = 0;
    if (!readBytes(UNIT_8SERVOS2_REG_ADDR_OFFSET, &offset, 1) || offset > 0x0F) return false;
    if (!writeByte(UNIT_8SERVOS2_REG_I2C_ADDR_REFRESH, 1)) return false;

    // The firmware applies the new address in its main loop after this write.
    delay(1);
    _addr = UNIT_8SERVOS2_DEFAULT_ADDR + offset;
    return true;
}

uint8_t M5Unit8Servos2::getI2CAddress()
{
    return readByte(UNIT_8SERVOS2_REG_I2C_ADDR);
}
