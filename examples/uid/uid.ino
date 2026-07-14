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

    uint8_t uid[UNIT_8SERVOS2_UID_LENGTH] = {0};
    servos2.getUID(uid, sizeof(uid));

    Serial.print("UID: ");
    for (uint8_t i = 0; i < sizeof(uid); i++) {
        Serial.printf("%02X", uid[i]);
    }
    Serial.println();
}

void loop()
{
}
