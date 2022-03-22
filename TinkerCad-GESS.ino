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

#define BATTERY_MIN_VOLTAGE 10.8;
#define BATTERY_MAX_VOLTAGE 13.6;
#define DIGITAL_CONSTANT 1023;

float temp = 0.0;
int baselineTemp = 0;
int celsius = 0;
boolean toggle = false;
// Weerstand constante
int const RESISTANCE = 1023;

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
    pinMode(TEMP_SENSOR, INPUT);
    pinMode(SOLAR_INPUT, INPUT);
    pinMode(8, OUTPUT);
    pinMode(BUZZER, OUTPUT);
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
    if (toggle == 1)
    {
        digitalWrite(BATTERY_RELAY, HIGH);
        digitalWrite(SOLAR_OUTPUT, LOW);
        digitalWrite(BATTERY_OUTPUT, HIGH);
        Serial.println("BATTERY ON");
    }

    if (toggle == 0)
    {
        Serial.println("SOLAR ON");
        digitalWrite(BATTERY_RELAY, LOW);
        digitalWrite(SOLAR_OUTPUT, HIGH);
        digitalWrite(BATTERY_OUTPUT, LOW);
    }

    // Time
    int time_elapsed = millis();
    Serial.print("time: ");
    Serial.println(time_elapsed);

    // Solar Panel
    int solar_resistance = analogRead(SOLAR_INPUT);
    float solar_voltage = calculateVoltage(solar_resistance);
    Serial.print("Solar Voltage: ");
    Serial.println(solar_voltage);
    Serial.println(RESISTANCE);

    // Battery Monitor
    int battery_resistance = analogRead(BATTERY_INPUT);
    float battery_voltage = calculateVoltage(battery_resistance);
    float battery_current = calculateCurrent(battery_voltage);

    Serial.print("Battery Voltage: ");
    Serial.println(battery_voltage);
    Serial.print("Battery Current: ");
    Serial.println(battery_current);

    // Battery Charger
    // float battery_capacity = battery_current * time_elapsed;
    // Serial.print("Battery Capacity: ");
    // Serial.println(battery_capacity);

    // Voltmeter
    int analog_value = analogRead(A0);
    temp = (analog_value * 5.0) / 1023.0;

    int Temp = 3;

    Serial.print("v= ");
    Serial.println(temp);

    lcd.setCursor(0, 0);
    lcd.print("Solar:");
    lcd.print(solar_voltage);

    // Print
    lcd.setCursor(0, 1);
    lcd.print("Battery:");
    lcd.print(solar_voltage);

    delay(1000);
    toggle = !toggle;
}

float calculateVoltage(int resistance)
{
    return ((resistance * 5.0) / RESISTANCE) * 5.015;
}

float calculateCurrent(float voltage)
{
    return voltage / RESISTANCE;
}

// vanaf hier begint het checken van connecties aan de GESS
bool solarArrayConnected(float voltage)
{
    if (voltage != 0)
    {
        lcd.setCursor(0, 1);
        lcd.print("Solar Array Connected");
        return true;
    }
    lcd.setCursor(0, 1);
    lcd.print("Solar Array Not Connected");
    return false;
}

bool batteryConnected(float voltage)
{

    if (voltage != 0)
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
