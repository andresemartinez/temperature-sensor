#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <config.hpp>
#include <log.hpp>

long tick_start;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntp_server, utc_offset);

void setup_wifi() {
  logln("Starting WiFi setup");

  WiFi.begin(ssid, password);

  logln("Connecting ");

  while (WiFi.status() != WL_CONNECTED)
  {
     delay(500);
     log("*");
  }

  logln("");
  log("WiFi connection Successful (");
  log(WiFi.localIP());
  logln(")");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    log("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(client_id)) {
      logln("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
    } else {
      log("failed, rc=");
      log(client.state());
      logln(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void tick() {
  log("Ticking...");
  logln(timeClient.getEpochTime());

  log(daysOfTheWeek[timeClient.getDay()]);
  log(", ");
  log(timeClient.getHours());
  log(":");
  log(timeClient.getMinutes());
  log(":");
  logln(timeClient.getSeconds());

  if (!client.connected()) {
    reconnect();
  }

  long now = millis();
  if (now - lastMsg > 2000) { // TODO Check this
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    log("Publish message: ");
    logln(msg);
    client.publish("temperature_topic", msg);
  }
}

void setup() {
  initLog(serial_port);

  setup_wifi();

  timeClient.begin();

  client.setServer(mqtt_server, mqtt_port);

  tick_start = timeClient.getEpochTime();

}

void loop() {

  // Process inconming messages and keep alive
  // Should execute regularly (every loop)
  client.loop();

  // Send update notification to time client
  // This method only efectivly sends an NTP request every 60 seconds
  // This can be modified in the constructor
  timeClient.update();

  // Tick every 'tick_interval' seconds
  if(timeClient.getEpochTime() - tick_start > tick_interval) {
    long tick_start = timeClient.getEpochTime();
    log("Tick start: ");
    logln(tick_start);

    tick();

  }

}
