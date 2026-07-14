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
        servos2.setMode(ch, M5_8SERVOS2_MODE_RGB);
        Serial.println("Channel " + String(ch) + " Mode: " + String(servos2.getMode(ch)));
    }
}
void loop()
{
    uint32_t test_color[6] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFFFF, 0xFF00FF, 0x000000};

    for (uint8_t j = 0; j < 6; j++) {
        uint32_t colors[16] = {0};
        for (uint16_t i = 0; i < 16; i++) {
            colors[i] = test_color[j];
        }
        servos2.setRGBBuffer(colors, 16);
        for (uint16_t i = 0; i < 16; i++) {
            uint32_t read_color = servos2.getRGBBufferIndex(i);
            if (read_color != colors[i]) {
                Serial.printf("Error: Channel %d RGB: 0x%06X != 0x%06X\r\n", i, read_color, colors[i]);
            } else {
                Serial.printf("Channel %d RGB: 0x%06X == 0x%06X\r\n", i, read_color, colors[i]);
            }
        }

        for (uint8_t i = 0; i < 8; i++) {
            servos2.setRGBConfig(i, 16, true);
        }
        delay(1000);
    }
}
