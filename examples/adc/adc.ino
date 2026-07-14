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
        servos2.setMode(ch, M5_8SERVOS2_MODE_ADC);
        Serial.println("Channel " + String(ch) + " Mode: " + String(servos2.getMode(ch)));
    }
}
uint16_t adc_values[8];
uint16_t adc_voltage[8];
void loop()
{
    Serial.println("ADC Values:");
    for (uint8_t ch = 0; ch < 8; ch++) {
        adc_values[ch]  = servos2.getADCRaw(ch);
        adc_voltage[ch] = servos2.getVoltageMV(ch);
        Serial.printf("Channel %d ADC Value: %d, Voltage: %dmV\n", ch, adc_values[ch], adc_voltage[ch]);
    }
    delay(1000);
}
