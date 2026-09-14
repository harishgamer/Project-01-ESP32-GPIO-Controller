#define LED_PIN     2
#define BUTTON_PIN  17

// --------------------------------------------------
// PWM configuration
// --------------------------------------------------

#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 8

#define BRIGHTNESS_FULL 255
#define BRIGHTNESS_DIM  75

// --------------------------------------------------
// LED states
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

    // Configure PWM on LED pin
    ledcAttach(
        LED_PIN,
        PWM_FREQUENCY,
        PWM_RESOLUTION
    );

    ledcWrite(LED_PIN, 0);

    // Configure button
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    attachInterrupt(
        digitalPinToInterrupt(BUTTON_PIN),
        buttonISR,
        FALLING
    );

    Serial.println();
    Serial.println("=================================");
    Serial.println("ESP32 GPIO Level 4");
    Serial.println("PWM + State Machine");
    Serial.println("=================================");

    Serial.println("State: OFF");
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

    // Reset timing whenever state changes
    previousMillis = millis();

    // Reset blink state
    ledState = false;

    // Start new state with LED OFF
    ledcWrite(LED_PIN, 0);
}

// --------------------------------------------------
// Update LED
// --------------------------------------------------

void updateLed()
{
    unsigned long currentMillis = millis();

    switch (currentState)
    {
        // ------------------------------------------
        // OFF
        // ------------------------------------------

        case LED_OFF:

            ledcWrite(LED_PIN, 0);

            break;


        // ------------------------------------------
        // ON - 100% brightness
        // ------------------------------------------

        case LED_ON:

            ledcWrite(LED_PIN, BRIGHTNESS_FULL);

            break;


        // ------------------------------------------
        // SLOW BLINK - DIM
        // ------------------------------------------

        case LED_SLOW_BLINK:

            if (currentMillis - previousMillis >= 1000)
            {
                previousMillis = currentMillis;

                ledState = !ledState;

                if (ledState)
                {
                    ledcWrite(
                        LED_PIN,
                        BRIGHTNESS_DIM
                    );
                }
                else
                {
                    ledcWrite(
                        LED_PIN,
                        0
                    );
                }
            }

            break;


        // ------------------------------------------
        // FAST BLINK - FULL brightness
        // ------------------------------------------

        case LED_FAST_BLINK:

            if (currentMillis - previousMillis >= 250)
            {
                previousMillis = currentMillis;

                ledState = !ledState;

                if (ledState)
                {
                    ledcWrite(
                        LED_PIN,
                        BRIGHTNESS_FULL
                    );
                }
                else
                {
                    ledcWrite(
                        LED_PIN,
                        0
                    );
                }
            }

            break;
    }
}

// --------------------------------------------------
// Main loop
// --------------------------------------------------

void loop()
{
    if (buttonPressed)
    {
        buttonPressed = false;

        handleButtonPress();
    }

    updateLed();
}