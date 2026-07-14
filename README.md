# M5Unit 8Servos2 I2C

Arduino library for the M5Stack Unit 8Servos2, an I2C expansion unit that gives
eight configurable channels to an ESP32 host.

Each channel can operate as a digital input, digital output, ADC input, servo
output, RGB LED output, or PWM output. The library also exposes device
telemetry, firmware information, and the hardware UID.

### Unit 8Servos2 I2C (SKU:U222)

- 📖 [Documentation & Datasheet](https://docs.m5stack.com/en/products/sku/U224)

## Features

- Eight independently configurable channels
- Digital input with pull-up, pull-down, or no pull resistor
- Digital output and 12-bit ADC input
- Servo control from 0 to 180 degrees
- PWM output with two configurable timer groups
- RGB output with a shared 16-color buffer
- System voltage, current, firmware version, and UID readback
- Arduino IDE and PlatformIO metadata

## Compatibility

- Framework: Arduino
- Architecture: ESP32
- Default I2C address: `0x25`
- Default I2C clock: 400 kHz

## Installation

### Arduino IDE

Download this repository as a ZIP file, then select **Sketch > Include Library
> Add .ZIP Library...** in the Arduino IDE.

### PlatformIO

Add the repository to `lib_deps` in `platformio.ini`:

```ini
lib_deps =
    https://github.com/m5stack/M5Unit-8Servos2-I2C.git
```

The driver only depends on the Arduino `Wire` library included with the ESP32
core.

## Quick Start

Connect the unit to an ESP32 I2C port and update the SDA/SCL pin values for your
board:

```cpp
#include <M5Unit8Servos2.h>

M5Unit8Servos2 unit;

void setup()
{
    Serial.begin(115200);

    if (!unit.begin(&Wire, UNIT_8SERVOS2_DEFAULT_ADDR, 32, 33)) {
        Serial.println("Unit 8Servos2 not found");
        return;
    }

    unit.setMode(0, M5_8SERVOS2_MODE_SERVO);
    unit.setTimerFrequency(0, 50);
    unit.setServoAngle(0, 90);
}

void loop()
{
}
```

When the application has already initialized `Wire`, pass `-1` for both pin
arguments to keep the existing bus configuration:

```cpp
Wire.begin();
unit.begin(&Wire, UNIT_8SERVOS2_DEFAULT_ADDR, -1, -1);
```

## Examples

| Example | Description |
| --- | --- |
| [`adc`](examples/adc/adc.ino) | Read raw ADC values and millivolts |
| [`input`](examples/input/input.ino) | Read digital inputs with pull-ups |
| [`output`](examples/output/output.ino) | Toggle all digital outputs |
| [`pwm`](examples/pwm/pwm.ino) | Sweep PWM duty cycle on all channels |
| [`rgb`](examples/rgb/rgb.ino) | Fill and refresh the RGB buffer |
| [`servos`](examples/servos/servos.ino) | Sweep eight servo outputs |
| [`uid`](examples/uid/uid.ino) | Read the device UID |

## API Overview

| Area | Methods |
| --- | --- |
| Setup | `begin()` |
| Channel mode | `setMode()`, `getMode()` |
| Digital input | `setInputPull()`, `getInputPull()`, `getDigitalInput()` |
| Digital output | `setDigitalOutput()`, `getDigitalOutput()` |
| ADC | `getADCRaw()`, `getVoltageMV()` |
| Servo | `setServoAngle()`, `getServoAngle()` |
| PWM | `setPWMDuty()`, `getPWMDuty()`, `setTimerFrequency()`, `getTimerFrequency()` |
| RGB | `setRGBConfig()`, `getRGBConfig()`, `setRGBBufferIndex()`, `getRGBBufferIndex()`, `setRGBBuffer()`, `getRGBBuffer()` |
| Device information | `getUID()`, `getFirmwareVersion()`, `getI2CAddress()` |
| Telemetry | `getSysVREF()`, `getGroveVoltage()`, `getDCVoltage()`, `getSysCurrent()` |

Channels are numbered `0` through `7`. Timer group `0` controls channels 0-3,
and timer group `1` controls channels 4-7. Configure a channel with `setMode()`
before using its mode-specific methods.

## Development

The repository checks Arduino library compliance, formatting, and compilation
of every example on ESP32. To run the available local checks:

```sh
arduino-lint --compliance strict .
clang-format --dry-run --Werror src/* examples/*/*.ino
```

## License

This project is licensed under the [MIT License](LICENSE).
