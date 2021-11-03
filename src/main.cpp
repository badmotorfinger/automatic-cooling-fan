// Board: Arduino Nano
// Processor: AT Mega (Old bootloader)

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 12             // Digital pin connected to the DHT sensor
#define FANPIN 9
#define TEMP_LIMIT 30
#define CHECK_INTERVAL 600000 // 10 Minutes
#define DHTTYPE    DHT22      // DHT 22 (AM2302)

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup()
{

  Serial.begin(9600);

  // Initialize device.
  dht.begin();

  pinMode(FANPIN, OUTPUT);
  digitalWrite(FANPIN, LOW);

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));

  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));

  // Set delay between sensor readings based on sensor details.
  delayMS = (sensor.min_delay / 1000) * 2;
  Serial.print(F("Sensor read delay: ")); Serial.println(delayMS);
  delay(delayMS);
}

bool is_temp_over = false;

void loop() {

  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);

  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else
  {
    if (event.temperature > TEMP_LIMIT) {

      if (!is_temp_over)
      {
        is_temp_over = true;
        Serial.println(event.temperature);
        Serial.println("Turn on fan!!");
        digitalWrite(FANPIN, HIGH);
      }
    }
    else
    {
      is_temp_over = false;
      digitalWrite(FANPIN, LOW);
    }
  }

  delay(CHECK_INTERVAL);
}
