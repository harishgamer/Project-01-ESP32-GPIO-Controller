#define LED_PIN     2
#define BUTTON_PIN  17

// --------------------------------------------------
// State definitions
// --------------------------------------------------

enum LedState
{
    LED_OFF,
    LED_ON,
    LED_SLOW_BLINK,
    LED_FAST_BLINK
};

LedState currentState = LED_OFF;

// --------------------------------------------------
// Button interrupt
// --------------------------------------------------

volatile bool buttonPressed = false;

volatile unsigned long lastInterruptTime = 0;

const unsigned long debounceTime = 200;

// --------------------------------------------------
// LED timing
// --------------------------------------------------

unsigned long previousMillis = 0;

bool ledState = false;

// --------------------------------------------------
// Interrupt Service Routine
// --------------------------------------------------

void IRAM_ATTR buttonISR()
{
    unsigned long currentTime = millis();

    if (currentTime - lastInterruptTime > debounceTime)
    {
        buttonPressed = true;
        lastInterruptTime = currentTime;
    }
}

// --------------------------------------------------
// Setup
// --------------------------------------------------

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

    Serial.println();
    Serial.println("=================================");
    Serial.println("ESP32 GPIO Level 3");
    Serial.println("Finite State Machine");
    Serial.println("=================================");

    Serial.println("Current State: OFF");
}

// --------------------------------------------------
// Handle button event
// --------------------------------------------------

void handleButtonPress()
{
    switch (currentState)
    {
        case LED_OFF:
            currentState = LED_ON;
            Serial.println("State: OFF -> ON");
            break;

        case LED_ON:
            currentState = LED_SLOW_BLINK;
            Serial.println("State: ON -> SLOW BLINK");
            break;

        case LED_SLOW_BLINK:
            currentState = LED_FAST_BLINK;
            Serial.println("State: SLOW BLINK -> FAST BLINK");
            break;

        case LED_FAST_BLINK:
            currentState = LED_OFF;
            Serial.println("State: FAST BLINK -> OFF");
            break;
    }

    // Reset LED timing whenever state changes
    previousMillis = millis();

    // Reset LED output state
    ledState = false;

    digitalWrite(LED_PIN, LOW);
}

// --------------------------------------------------
// Run LED state machine
// --------------------------------------------------

void updateLed()
{
    unsigned long currentMillis = millis();

    switch (currentState)
    {
        // -----------------------------
        // OFF
        // -----------------------------

        case LED_OFF:

            digitalWrite(LED_PIN, LOW);

            break;


        // -----------------------------
        // ON
        // -----------------------------

        case LED_ON:

            digitalWrite(LED_PIN, HIGH);

            break;


        // -----------------------------
        // SLOW BLINK
        // -----------------------------

        case LED_SLOW_BLINK:

            if (currentMillis - previousMillis >= 1000)
            {
                previousMillis = currentMillis;

                ledState = !ledState;

                digitalWrite(LED_PIN, ledState);
            }

            break;


        // -----------------------------
        // FAST BLINK
        // -----------------------------

        case LED_FAST_BLINK:

            if (currentMillis - previousMillis >= 250)
            {
                previousMillis = currentMillis;

                ledState = !ledState;

                digitalWrite(LED_PIN, ledState);
            }

            break;
    }
}

// --------------------------------------------------
// Main loop
// --------------------------------------------------

void loop()
{
    // Check for button event
    if (buttonPressed)
    {
        buttonPressed = false;

        handleButtonPress();
    }

    // Continuously execute current state
    updateLed();
}