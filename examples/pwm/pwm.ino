#include <M5Unit8Servos2.h>

M5Unit8Servos2 servos2;

#define I2C_SDA_PIN (32)
#define I2C_SCL_PIN (33)

void setup()
{
    Serial.begin(115200);
    while (!servos2.begin(&Wire, UNIT_8SERVOS2_DEFAULT_ADDR, I2C_SDA_PIN, I2C_SCL_PIN, 400000)) {
        Serial.println("Failed to initialize M5Unit8Servos2. Retrying...");
        delay(100);
    }

    for (uint8_t ch = 0; ch < 8; ch++) {
        servos2.setMode(ch, M5_8SERVOS2_MODE_PWM);
        Serial.println("Channel " + String(ch) + " Mode: " + String(servos2.getMode(ch)));
    }

    servos2.setTimerFrequency(0, 1000);
    servos2.setTimerFrequency(1, 1000);
}
uint8_t pwm_duty = 0;
void loop()
{
    for (uint8_t ch = 0; ch < 8; ch++) {
        servos2.setPWMDuty(ch, pwm_duty);
    }
    pwm_duty += 10;
    if (pwm_duty > 100) {
        pwm_duty = 0;
    }
    delay(5000);
}
