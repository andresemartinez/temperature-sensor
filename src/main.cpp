#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Ticker.h>
#include <DHT.h>

#include <ntptime.h>
#include <report.h>
#include <config.h>
#include <debug-tools.h>

// WiFi client
WiFiClient wifiClient;

// MQTT client
PubSubClient mqttClient(wifiClient);

// NTP time client
WiFiUDP wifiUDP;
NtpTimeClient timeClient(wifiUDP);

// Temperature and Humidity sensors
Ticker sensorTicker;
DHT22 dhtSensor;
volatile float lastMeasurementTimestamp = 0;
volatile float measuredHumidity = 0;
volatile float measuredTemperature = 0;
volatile int8_t dhtResult = 0;

void readDht() {
  dhtSensor.read();
}

// This callback will be called from an interrupt
// It should be short and carry the ICACHE_RAM_ATTR attribute
void ICACHE_RAM_ATTR handleDhtData(float h, float t) {
  lastMeasurementTimestamp = timeClient.epochTime();
  measuredHumidity = h;
  measuredTemperature = t;
  dhtResult = 1;
}

// This callback will be called from an interrupt
// It should be short and carry the ICACHE_RAM_ATTR attribute
void ICACHE_RAM_ATTR handleDhtError(uint8_t e) {
  dhtResult = -1;
}

void mqttReconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    debugLog("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(mqtt_client_id)) {
      debugLogLn("connected.");
    } else {
      debugLog("failed, rc=");
      debugLog(mqttClient.state());
      debugLogLn(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setupWiFi() {
  debugLogLn("Starting WiFi.");

  WiFi.begin(wifi_ssid, wifi_password);

  debugLogLn("Connecting ");

  while (WiFi.status() != WL_CONNECTED)
  {
     delay(500);
     debugLog("*");
  }

  debugLogLn("");
  debugLog("WiFi connection Successful (");
  debugLog(WiFi.localIP());
  debugLogLn(").");

}

void setupTime() {
  debugLogLn("Starting timeClient client.");

  timeClient.begin();

}

void setupMqtt() {
  debugLogLn("Starting mqtt client.");

  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setBufferSize(mqtt_buffer_size);

  mqttReconnect();

}

void setupSensors() {
  debugLogLn("Starting sensors polling timer.");

  dhtSensor.setPin(dht_pin);
  dhtSensor.onData(handleDhtData);
  dhtSensor.onError(handleDhtError);
  sensorTicker.attach_scheduled(dht_read_interval, readDht);

}

WeatherReport* generateWeatherRepot(const int temperature, const int humidity) {

  debugLogLn("Creating weather report.");

  Sensor* tempSensor = new Sensor(
    SensorType::temperature,
    measuredTemperature,
    "C",
    timeClient.isoString(lastMeasurementTimestamp)
  );

  Sensor* humidSensor = new Sensor(
    SensorType::humidity,
    measuredHumidity,
    "%",
    timeClient.isoString(lastMeasurementTimestamp)
  );

  Sensor** sensors = (Sensor**) malloc(sizeof(Sensor*) * 2);
  sensors[0] = tempSensor;
  sensors[1] = humidSensor;

  WeatherReport* report = new WeatherReport(
    mqtt_client_id,
    timeClient.isoString(),
    sensors,
    2
  );

  debugLogLn("Weather report successfully created.");

  return report;
}

void sendWeatherReport(const WeatherReport* report) {
  debugLogLn("Sending weather report.");

  if (!mqttClient.connected()) {
    mqttReconnect();
  }

  ReportCommons::Json_t jsonReport = report->toJson();
  debugLog("Json: ");
  debugLogLn(jsonReport);

  mqttClient.publish(mqtt_weather_report_topic, jsonReport);

  debugLogLn("Weather report sent.");

  free(jsonReport);
}

void processDhtResult() {
  debugLog("Processing DHT result (");
  debugLog(dhtResult);
  debugLog(") at ")
  debugDo(char* isoDateTime = timeClient.isoString());
  debugLogLn(isoDateTime);
  debugDo(free(isoDateTime));

  if (dhtResult > 0) {
    debugLog("Humid: ");
    debugLog(measuredHumidity);
    debugLogLn("%.");
    debugLog("Temp: ");
    debugLog(measuredTemperature);
    debugLogLn(" C.");

    WeatherReport* report = generateWeatherRepot(measuredTemperature, measuredHumidity);
    sendWeatherReport(report);

    delete report;

   } else if (dhtResult < 0) {

     debugLog("Error: ");
     debugLogLn(dhtSensor.getError());

   }

   dhtResult = 0;

}

void setup() {

  initDebugLog(serial_port);

  debugLogLn("---------------------Setting up---------------------");

  setupWiFi();
  setupTime();
  setupMqtt();
  setupSensors();

  debugLogLn("---------------------Setup finished---------------------");
  debugLogLn("Looping...");

}

void loop() {

  // Process inconming messages and keep alive
  // Should execute regularly (every loop)
  mqttClient.loop();

  // Process sensor measurement
  if(dhtResult != 0) {
    processDhtResult();
  }

}
