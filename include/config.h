#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Debug mode (works with debug tools).
// Should comment this for production.
#define DEBUG

// Serial port
const int serial_port = 9600;

// WiFi Config
const char* wifi_ssid = "your_wifi_ssid";
const char* wifi_password = "yout_wifi_password";

// mqtt server
const char* mqtt_server = "mqtt_server_ip";
const int mqtt_port = 1883;
const uint16_t mqtt_buffer_size = 1024; // In bytes
const char* mqtt_client_id = "weather-station-1";
const char* mqtt_weather_report_topic = "weatherreport";

// DHT22
const int dht_pin = D1; // Digital pin connected to the DHT sensor
const int dht_read_interval = 30; // In seconds

#endif
