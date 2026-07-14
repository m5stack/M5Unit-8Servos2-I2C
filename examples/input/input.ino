#include <M5Unit8Servos2.h>

M5Unit8Servos2 servos2;

#define I2C_SDA_PIN (32)
#define I2C_SCL_PIN (33)

void setup()
{
    Serial.begin(115200);
    while (!servos2.begin(&Wire, UNIT_8SERVOS2_DEFAULT_ADDR, I2C_SDA_PIN, I2C_SCL_PIN, 400000)) {
        Serial.println("Failed to initialize M5Unit8Servos2. Retrying...");
        delay(10);
    }

    for (uint8_t ch = 0; ch < 8; ch++) {
        servos2.setMode(ch, M5_8SERVOS2_MODE_INPUT);
        servos2.setInputPull(ch, M5_8SERVOS2_PULL_UP);
        // servos2.setInputPull(ch, M5_8SERVOS2_PULL_DOWN);
        Serial.println("Channel " + String(ch) + " Mode: " + String(servos2.getMode(ch)));
    }
}

void loop()
{
    Serial.println("Reading Digital Inputs:");
    for (uint8_t ch = 0; ch < 8; ch++) {
        bool level = servos2.getDigitalInput(ch);
        Serial.printf("Channel %d Input Level: %d\n", ch, level);
    }
    delay(1000);
}
