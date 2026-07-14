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
        servos2.setMode(ch, M5_8SERVOS2_MODE_OUTPUT);
        Serial.println("Channel " + String(ch) + " Mode: " + String(servos2.getMode(ch)));
    }
}
uint8_t output_level = 0;
void loop()
{
    Serial.println("Output Level: " + String(output_level));
    for (uint8_t ch = 0; ch < 8; ch++) {
        servos2.setDigitalOutput(ch, output_level);
        Serial.printf("Channel %d Output Level: %d\n", ch, output_level);
    }
    output_level = !output_level;
    delay(1000);
}
