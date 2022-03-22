// include the library code:
#include <LiquidCrystal.h>
#define RED_LED 13
#define ORANGE_LED 12
#define YELLOW_LED 11
#define TEMP_SENSOR A1
#define SOLAR_INPUT A2
#define SOLAR_OUTPUT 7
#define BATTERY_INPUT A3
#define BATTERY_OUTPUT 6
#define SOLAR_OUTPUT 7
#define BATTERY_RELAY 8
#define BUZZER 9

#define BATTERY_MIN_VOLTAGE 10.8
#define BATTERY_MAX_VOLTAGE 13.6
#define SOlAR_MIN_CHARGE_VOLTAGE 12.9
#define DIGITAL_CONSTANT 1023

boolean relay_toggle = true;
boolean battery_connected = false;
boolean solar_connected = false;

// Functions
float calculateVoltage(), calculateCurrent();

LiquidCrystal lcd(27, 26, 5, 4, 3, 2, 1);

void setup()
{
    Serial.begin(9600);
    lcd.begin(16, 2);
    pinMode(ORANGE_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(SOLAR_INPUT, INPUT);
    pinMode(BATTERY_RELAY, OUTPUT);
    pinMode(SOLAR_OUTPUT, OUTPUT);
    pinMode(BATTERY_OUTPUT, OUTPUT);

    digitalWrite(BUZZER, HIGH);
    delay(1000);
    digitalWrite(BUZZER, LOW);

    int battery_reading = analogRead(BATTERY_INPUT);
    int solar_reading = analogRead(SOLAR_INPUT);
    float battery_voltage = calculateVoltage(battery_reading);
    float solar_voltage = calculateVoltage(solar_reading);

    solarArrayConnected(solar_voltage);
    batteryConnected(battery_voltage);
}

// todo: add comments
void loop()
{
    if (relay_toggle == 1)
    {
        digitalWrite(BATTERY_RELAY, HIGH);
        digitalWrite(SOLAR_OUTPUT, LOW);
        digitalWrite(BATTERY_OUTPUT, HIGH);
        Serial.println("BATTERY ON");
    }

    if (relay_toggle == 0)
    {
        Serial.println("SOLAR ON");
        digitalWrite(BATTERY_RELAY, LOW);
        digitalWrite(SOLAR_OUTPUT, HIGH);
        digitalWrite(BATTERY_OUTPUT, LOW);
    }

    // Solar Panel
    int solar_reading = analogRead(SOLAR_INPUT);
    float solar_voltage = calculateVoltage(solar_reading);

    // Battery Values
    int battery_reading = analogRead(BATTERY_INPUT);
    float battery_voltage = calculateVoltage(battery_reading);
    float battery_percentage = calculateBatteryPercentage(battery_voltage);

    // Print values to console
    printPowerLevelsToConsole(solar_voltage, battery_voltage, battery_percentage);

    // Print values to lcd
    printPowerLevelsToLcd(solar_voltage, battery_voltage, battery_percentage);

    // Monitor power
    monitorPowerLevels(solar_voltage, battery_voltage);

    relay_toggle = !relay_toggle;
}

void monitorPowerLevels(float solar_voltage, float battery_voltage)
{
    // mode: Battery Charging
    if (solar_voltage >= SOlAR_MIN_CHARGE_VOLTAGE && battery_voltage <= BATTERY_MIN_VOLTAGE)
    {
        digitalWrite(BATTERY_RELAY, HIGH);
        digitalWrite(BATTERY_RELAY, HIGH);
    }

    // mode: Battery Charging
    // if (solar_voltage >= SOlAR_MIN_CHARGE_VOLTAGE && battery_voltage <= BATTERY_MIN_VOLTAGE)
    // {
    //   digitalWrite(BATTERY_RELAY, HIGH);
    //   digitalWrite(BATTERY_RELAY, HIGH);
    // }
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
        lcd.setCursor(0, 1);
        lcd.print("Solar Array Connected");
        Serial.println("Solar Array Connected");
        return true;
    }
    lcd.setCursor(0, 1);
    lcd.print("Solar Array Not Connected");
    Serial.println("Solar Array Not Connected");
    return false;
}

bool batteryConnected(float voltage)
{

    if (voltage >= 10.8)
    { // insert logic here - coming soon
        lcd.setCursor(0, 1);
        lcd.print("Battery Connected");
        return true;
    }
    lcd.setCursor(0, 1);
    lcd.print("Battery Not Connected");
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
