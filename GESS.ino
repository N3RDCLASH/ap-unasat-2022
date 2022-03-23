// include the library code:
#include <LiquidCrystal_I2C.h>

// arduino pin configuration
#define RED_LED 13
#define BLUE_LED 12
#define YELLOW_LED 11
#define SOLAR_INPUT A2
#define SOLAR_OUTPUT 7
#define BATTERY_INPUT A3
#define BATTERY_OUTPUT 6
#define SOLAR_OUTPUT 7
#define BATTERY_RELAY 8
#define BUZZER 9

#define BATTERY_MIN_VOLTAGE 10.8
#define BATTERY_MAX_VOLTAGE 13.6
#define SOLAR_MIN_CHARGE_VOLTAGE 12.9
#define DIGITAL_CONSTANT 1023

boolean relay_toggle = true;
boolean battery_connected = false;
boolean solar_connected = false;
boolean setup_completed = false;

// Functions
float calculateVoltage(), calculateCurrent();

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    pinMode(BLUE_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(SOLAR_INPUT, INPUT);
    pinMode(BATTERY_RELAY, OUTPUT);
    pinMode(SOLAR_OUTPUT, OUTPUT);
    pinMode(BATTERY_OUTPUT, OUTPUT);

    digitalWrite(RED_LED, HIGH);

    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);

    digitalWrite(BATTERY_RELAY, HIGH);

    setup_completed = true;
}

// todo: add comments
void loop()
{
    // Solar Panel
    int solar_reading = analogRead(SOLAR_INPUT);
    float solar_voltage = calculateVoltage(solar_reading);

    // Battery Values
    int battery_reading = analogRead(BATTERY_INPUT);
    float battery_voltage = calculateVoltage(battery_reading);
    float battery_percentage = calculateBatteryPercentage(battery_voltage);

    solar_connected = solarArrayConnected(solar_voltage);
    battery_connected = batteryConnected(battery_voltage);

    while (!solar_connected || !battery_connected)
    {
        digitalWrite(RED_LED, HIGH);
        delay(200);
        digitalWrite(RED_LED, LOW);
        delay(200);
        return;
    }

    // Print values to console
    printPowerLevelsToConsole(solar_voltage, battery_voltage, battery_percentage);

    // Print values to lcd
    printPowerLevelsToLcd(solar_voltage, battery_voltage, battery_percentage);

    // Monitor power
    monitorPowerLevels(solar_voltage, battery_voltage);

    Serial.println("---------");
}

void monitorPowerLevels(float solar_voltage, float battery_voltage)
{
    // mode: Battery Charging
    if (solar_voltage >= SOLAR_MIN_CHARGE_VOLTAGE)
    {
        turnOnSolarIndicatorLED();
        if (BATTERY_MIN_VOLTAGE < battery_voltage < BATTERY_MAX_VOLTAGE)
        {
            // connect the solar voltage to charge the battery
            digitalWrite(BATTERY_RELAY, HIGH);
            // set solar panel as output
            digitalWrite(SOLAR_OUTPUT, HIGH);
            // remove battery as output
            digitalWrite(BATTERY_OUTPUT, LOW);
            return;
        }

        if (battery_voltage == BATTERY_MAX_VOLTAGE)
        {
            // disconnect the solar voltage
            digitalWrite(BATTERY_RELAY, LOW);
            // set solar panel as output
            digitalWrite(SOLAR_OUTPUT, HIGH);
            // remove battery as output
            digitalWrite(BATTERY_OUTPUT, LOW);
            return;
        }
    }

    // mode: Battery Charging
    if (solar_voltage < SOLAR_MIN_CHARGE_VOLTAGE)
    {
        turnOnBatteryIndicatorLED();
        // disconnect the solar voltage to charge the battery
        digitalWrite(BATTERY_RELAY, LOW);

        if (battery_voltage == BATTERY_MAX_VOLTAGE)
        {
            // set solar panel as output
            digitalWrite(SOLAR_OUTPUT, HIGH);
            // remove battery as output
            digitalWrite(BATTERY_OUTPUT, LOW);
            return;
        }
    }
}

void printPowerLevelsToConsole(float solar_voltage, float battery_voltage, float battery_percentage)
{
    // Print values to console (Serial Monitor)
    Serial.print("Solar Voltage: ");
    Serial.println(solar_voltage);
    Serial.print("Battery Voltage: ");
    Serial.println(battery_voltage);
    Serial.print("Battery Percentage: ");
    Serial.println(battery_percentage);
}

void printPowerLevelsToLcd(float solar_voltage, float battery_voltage, float battery_percentage)
{
    lcd.setCursor(0, 0);
    lcd.print("Solar:");
    lcd.print(solar_voltage);

    lcd.setCursor(0, 1);
    lcd.print("Battery:");
    lcd.print(battery_voltage);
}

void turnOnSolarIndicatorLED()
{
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
}

void turnOnBatteryIndicatorLED()
{
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);
}

// start calculation methods
float calculateVoltage(int reading)
{
    return ((reading * 5.0) / DIGITAL_CONSTANT) * 5.5;
}

float calculateCurrent(float voltage)
{
    return voltage / DIGITAL_CONSTANT;
}

float calculateBatteryPercentage(float current_voltage)
{
    return ((current_voltage - BATTERY_MIN_VOLTAGE) / (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE)) * 100;
}
// end calculation methods

// vanaf hier begint het checken van connecties aan de GESS
bool solarArrayConnected(float voltage)
{
    if (voltage != 0)
    {
        if (!setup_completed)
        {
            lcd.setCursor(0, 1);
            lcd.print("Solar Array Connected");
            Serial.println("Solar Array Connected");
        }
        return true;
    }
    lcd.setCursor(0, 1);
    lcd.print("Solar Array Not Connected");
    Serial.println("Solar Array Not Connected");
    return false;
}

bool batteryConnected(float voltage)
{
    if (voltage >= BATTERY_MIN_VOLTAGE)
    { // insert logic here - coming soon
        if (!setup_completed)
        {
            lcd.setCursor(0, 1);
            lcd.print("Battery Connected");
            Serial.println("Battery Connected");
        }
        return true;
    }
    lcd.setCursor(0, 1);
    lcd.print("Battery Not Connected");
    Serial.println("Battery Not Connected");
    return false;
}

bool outputWires()
{ // L+- komt hier
    if (true)
    { // insert logic here -coming soon
        lcd.setCursor(0, 1);
        lcd.print("Output wires connected");
        return true;
    }
    lcd.setCursor(0, 1);
    lcd.print("Output Wires Not Connected");
    return false;
}