// include the library code:
#include <LiquidCrystal_I2C.h>

// arduino pin configuration
#define RED_LED 13
#define ORANGE_LED 12
#define YELLOW_LED 11
#define SOLAR_INPUT A2
#define BATTERY_INPUT A1
#define RELAY 8
#define BUZZER 9
#define BATTERY_OUTPUT 6
#define SOLAR_OUTPUT 7
#define BATTERY_RELAY 8
// end arduino pin configuration

#define BATTERY_MIN_VOLTAGE 10.8;
#define BATTERY_MAX_VOLTAGE 13.6;
#define DIGITAL_CONSTANT 1023;

boolean relay_toggle = true;

// Functions
float calculateVoltage(), calculateCurrent(), calculateBatteryPercentage();

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(ORANGE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(SOLAR_INPUT, INPUT);
  pinMode(RELAY, OUTPUT);

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
    digitalWrite(8, HIGH);
    Serial.println("Relay Aan.");
  }
  if (relay_toggle == 0)
  {
    digitalWrite(8, LOW);
    Serial.println("Relay Uit.");
  }

  // Time
  int time_elapsed = millis();
  Serial.print("time: ");
  Serial.println(time_elapsed);

  // Solar Panel
  int solar_reading = analogRead(SOLAR_INPUT);
  float solar_voltage = calculateVoltage(solar_reading);

  Serial.print("Solar Voltage: ");
  Serial.println(solar_voltage);

  // Battery Monitor
  int battery_reading = analogRead(BATTERY_INPUT);
  float battery_voltage = calculateVoltage(battery_reading);
  float battery_percentage = calculateBatteryPercentage(battery_voltage);

  Serial.print("Battery Voltage: ");
  Serial.println(battery_voltage);
  Serial.print("Battery Percentage: ");
  Serial.println(battery_percentage);

  lcd.setCursor(0, 0);
  lcd.print("Solar:");
  lcd.print(solar_voltage);

  // Print
  lcd.setCursor(0, 1);
  lcd.print("Battery:");
  lcd.print(battery_voltage);

  delay(2000);
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
