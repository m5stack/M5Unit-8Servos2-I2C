/*
 * SPDX-FileCopyrightText: 2026 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef UNIT_8SERVOS2_HPP
#define UNIT_8SERVOS2_HPP

#include "Arduino.h"
#include "Wire.h"

// #define MODULE_8SERVOS2_DEBUG Serial
#if defined MODULE_8SERVOS2_DEBUG
#define serialPrint(...)   MODULE_8SERVOS2_DEBUG.print(__VA_ARGS__)
#define serialPrintln(...) MODULE_8SERVOS2_DEBUG.println(__VA_ARGS__)
#define serialPrintf(...)  MODULE_8SERVOS2_DEBUG.printf(__VA_ARGS__)
#define serialFlush()      MODULE_8SERVOS2_DEBUG.flush()
#else
#define serialPrint(...)
#define serialPrintln(...)
#define serialPrintf(...)
#define serialFlush()
#endif

// Register Definitions / 寄存器定义
#define UNIT_8SERVOS2_REG_MODE_START          (0x00)
#define UNIT_8SERVOS2_REG_INPUT_PULL_START    (0x10)
#define UNIT_8SERVOS2_REG_INPUT_STATUS_START  (0x20)
#define UNIT_8SERVOS2_REG_OUTPUT_STATUS_START (0x30)
#define UNIT_8SERVOS2_REG_ADC_START           (0x40)
#define UNIT_8SERVOS2_REG_SERVO_START         (0x50)
#define UNIT_8SERVOS2_REG_RGB_CONF_START      (0x60)
#define UNIT_8SERVOS2_REG_PWM_DUTY_START      (0x70)
#define UNIT_8SERVOS2_REG_RGB_BUFFER_START    (0x80)
#define UNIT_8SERVOS2_REG_TIM_FREQ_START      (0xD0)
#define UNIT_8SERVOS2_REG_UID_START           (0xE0)
#define UNIT_8SERVOS2_REG_UID_END             (0xEB)
#define UNIT_8SERVOS2_REG_SYS_ANALOG_START    (0xF0)
#define UNIT_8SERVOS2_REG_VERSION             (0xFE)
#define UNIT_8SERVOS2_REG_I2C_ADDR            (0xFF)

#define UNIT_8SERVOS2_UID_LENGTH   (UNIT_8SERVOS2_REG_UID_END - UNIT_8SERVOS2_REG_UID_START + 1)
#define UNIT_8SERVOS2_DEFAULT_ADDR (0x25)

/**
 * @brief Enumeration of supported GPIO operation modes.
 *        支持的 GPIO 操作模式枚举。
 */
typedef enum {
    M5_8SERVOS2_MODE_INPUT  = 0x00,  // Input mode / 输入模式
    M5_8SERVOS2_MODE_OUTPUT = 0x01,  // Output mode / 输出模式
    M5_8SERVOS2_MODE_ADC    = 0x02,  // Analog/ADC mode / 模拟/ADC 模式
    M5_8SERVOS2_MODE_SERVO  = 0x03,  // Servo control mode / 舵机控制模式
    M5_8SERVOS2_MODE_RGB    = 0x04,  // RGB LED control mode / RGB LED 控制模式
    M5_8SERVOS2_MODE_PWM    = 0x05,  // PWM output mode / PWM 输出模式
} m5_8servo2_mode_t;

/**
 * @brief Enumeration of GPIO pull resistor configurations.
 *        GPIO 上拉/下拉电阻配置枚举。
 */
typedef enum {
    M5_8SERVOS2_PULL_NONE = 0x00,  // No pull-up/pull-down / 无上下拉
    M5_8SERVOS2_PULL_UP   = 0x01,  // Pull-up resistor enabled / 启用上拉电阻
    M5_8SERVOS2_PULL_DOWN = 0x02,  // Pull-down resistor enabled / 启用下拉电阻
} m5_8servo2_pull_t;

class M5Unit8Servos2 {
public:
    /**
     * @brief Initializes the device. / 初始化设备。
     * @param wire Pointer to TwoWire instance
     * @param addr I2C Address (default 0x25)
     * @param sda SDA Pin (use -1 to skip Wire.begin)
     * @param scl SCL Pin (use -1 to skip Wire.begin)
     * @param speed I2C Frequency
     */
    bool begin(TwoWire *wire = &Wire, uint8_t addr = UNIT_8SERVOS2_DEFAULT_ADDR, int sda = 21, int scl = 22,
               uint32_t speed = 400000L);

    /**
     * @brief Set the operation mode for a specific channel.
     *        设置指定通道的操作模式。
     * @param channel 0-7
     */
    void setMode(uint8_t channel, m5_8servo2_mode_t mode);

    /**
     * @brief Get the operation mode for a specific channel.
     *        获取指定通道的操作模式。
     */
    m5_8servo2_mode_t getMode(uint8_t channel);

    /**
     * @brief Set input pull resistor mode.
     *        设置输入上拉/下拉电阻模式。
     */
    void setInputPull(uint8_t channel, m5_8servo2_pull_t pull);

    /**
     * @brief Get input pull resistor mode.
     *        获取输入上拉/下拉电阻模式。
     */
    m5_8servo2_pull_t getInputPull(uint8_t channel);

    /**
     * @brief Get the digital input level.
     *        获取数字输入电平。
     */
    bool getDigitalInput(uint8_t channel);

    /**
     * @brief Set the digital output level.
     *        设置数字输出电平。
     */
    void setDigitalOutput(uint8_t channel, bool level);

    /**
     * @brief Get the digital output level.
     *        获取数字输出电平。
     */
    bool getDigitalOutput(uint8_t channel);

    /**
     * @brief Get raw ADC value (12-bit).
     *        获取原始 ADC 值 (12位)。
     */
    uint16_t getADCRaw(uint8_t channel);

    /**
     * @brief Get calculated voltage in mV.
     *        获取计算后的电压值 (mV)。
     */
    uint16_t getVoltageMV(uint8_t channel);

    /**
     * @brief Set Servo angle.
     *        设置舵机角度。
     * @param angle 0-180 degrees
     */
    void setServoAngle(uint8_t channel, uint8_t angle);

    /**
     * @brief Get Servo angle.
     *        获取舵机角度。
     */
    uint8_t getServoAngle(uint8_t channel);

    /**
     * @brief Set RGB LED color and refresh.
     *        设置 RGB LED 颜色并刷新。
     * @param channel GPIO Channel (0-7) configured as RGB
     * @param num_leds Number of LEDs to update (1-16)
     */
    void setRGBConfig(uint8_t channel, uint8_t num_leds, bool refresh = false);

    /**
     * @brief Get RGB LED configuration.
     *        获取 RGB LED 配置。
     */
    uint8_t getRGBConfig(uint8_t channel);

    /**
     * @brief Set color in the buffer at a specific index.
     *        在指定索引处设置缓冲区颜色（不立即刷新）。
     */
    void setRGBBufferIndex(uint8_t buffer_index, uint32_t color);

    /**
     * @brief Get color from the buffer at a specific index.
     *        获取缓冲区指定索引处的颜色。
     */
    uint32_t getRGBBufferIndex(uint8_t buffer_index);

    /**
     * @brief Set RGB LED buffer with colors.
     *        设置 RGB LED 缓冲区颜色。
     */
    void setRGBBuffer(const uint32_t *colors, uint8_t num_leds);

    /**
     * @brief Get RGB LED buffer colors.
     *        获取 RGB LED 缓冲区颜色。
     */
    void getRGBBuffer(uint32_t *colors, uint8_t num_leds);

    /**
     * @brief Set PWM duty cycle.
     *        设置 PWM 占空比。
     * @param duty 0-100 %
     */
    void setPWMDuty(uint8_t channel, uint8_t duty);

    /**
     * @brief Get PWM duty cycle.
     *        获取 PWM 占空比。
     */
    uint8_t getPWMDuty(uint8_t channel);

    /**
     * @brief Set the frequency for servo/PWM timers.
     *        设置舵机/PWM 定时器的频率。
     * @param timer_group 0: TIM2 (Ch 0-3), 1: TIM3 (Ch 4-7)
     */
    void setTimerFrequency(uint8_t timer_group, uint16_t freq);

    /**
     * @brief Get the frequency for servo/PWM timers.
     *        获取舵机/PWM 定时器的频率。
     */
    uint16_t getTimerFrequency(uint8_t timer_group);

    /**
     * @brief Get device unique ID data.
     *        获取设备唯一标识符数据。
     * @param uid Output data array.
     * @param length Number of bytes to read (maximum UNIT_8SERVOS2_UID_LENGTH).
     */
    void getUID(uint8_t uid[], uint8_t length);

    /**
     * @brief Get system reference voltage.
     *        获取系统参考电压 (mV)。
     */
    uint16_t getSysVREF();

    /**
     * @brief Get Grove port voltage.
     *        获取 Grove 接口电压 (mV)。
     */
    uint16_t getGroveVoltage();

    /**
     * @brief Get DC voltage.
     *        获取直流电压 (mV)。
     */
    uint16_t getDCVoltage();

    /**
     * @brief Get system current.
     *        获取系统电流 (mA)。
     */
    uint16_t getSysCurrent();

    /**
     * @brief Get firmware version.
     *        获取固件版本。
     */
    uint8_t getFirmwareVersion();

    /**
     * @brief Get I2C address.
     *        获取 I2C 地址。
     */
    uint8_t getI2CAddress();

private:
    TwoWire *_wire = nullptr;
    uint8_t _addr  = UNIT_8SERVOS2_DEFAULT_ADDR;

    // Internal I2C helpers
    bool writeBytes(uint8_t reg, const uint8_t *buffer, uint8_t length);
    bool readBytes(uint8_t reg, uint8_t *buffer, uint8_t length);
    bool writeByte(uint8_t reg, uint8_t data);
    uint8_t readByte(uint8_t reg);
};

#endif  // UNIT_8SERVOS2_HPP
