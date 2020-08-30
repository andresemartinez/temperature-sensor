#ifndef CONFIG_H
#define CONFIG_H

// Debug mode (enable logs).
// Should comment this for production.
#define DEBUG

// Serial port
const int serial_port = 9600;

// WiFi Config
const char* ssid = "yout_wifi_ssid";
const char* password = "your_wifi_password";

// mqtt server
const char* mqtt_server = "mqtt_server_ip";
const int mqtt_port = 1883;
const char* client_id = "temp_sensor_1-";
const char* temperature_topic = "prueba";

// NTP
const char* ntp_server = "pool.ntp.org";
const long utc_offset = 0; // In seconds

// Loop
const long tick_interval = 5; // In seconds

#endif
