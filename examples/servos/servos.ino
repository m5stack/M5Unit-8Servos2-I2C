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
        servos2.setMode(ch, M5_8SERVOS2_MODE_SERVO);
        Serial.println("Channel " + String(ch) + " Mode: " + String(servos2.getMode(ch)));
    }

    servos2.setTimerFrequency(0, 50);
    servos2.setTimerFrequency(1, 50);
}
uint8_t angle = 0;
void loop()
{
    for (uint8_t ch = 0; ch < 8; ch++) {
        servos2.setServoAngle(ch, angle);
    }
    angle += 20;
    if (angle > 180) {
        angle = 0;
    }
    delay(1000);
}
