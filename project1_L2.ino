#define LED_PIN     2
#define BUTTON_PIN  17

volatile bool buttonPressed = false;

unsigned long lastInterruptTime = 0;
const unsigned long debounceTime = 200;

void IRAM_ATTR buttonISR()
{
    unsigned long currentTime = millis();

    if (currentTime - lastInterruptTime > debounceTime)
    {
        buttonPressed = true;
        lastInterruptTime = currentTime;
    }
}

void setup()
{
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    attachInterrupt(
        digitalPinToInterrupt(BUTTON_PIN),
        buttonISR,
        FALLING
    );

    Serial.println("ESP32 GPIO Level 2 Started");
    Serial.println("Connect GPIO 27 to GND to toggle LED");
}

void loop()
{
    if (buttonPressed)
    {
        buttonPressed = false;

        digitalWrite(LED_PIN, !digitalRead(LED_PIN));

        Serial.println("Button press detected");
        Serial.println("LED state changed");
    }
}