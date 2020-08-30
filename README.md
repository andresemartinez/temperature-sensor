# Temperature Sensor

Temperature sensor made with the Arduino framework.
**Note**: This project was done as a hobby and was not tested for professional use.

## Config (config.hpp)

```
// Debug mode (enable logs).
// Should comment this for production use.
#define DEBUG

// Serial port
const int serial_port = 9600;

// WiFi
const char* ssid = "your_wifi_ssid";
const char* password = "yout_wifi_password";

// mqtt server
const char* mqtt_server = "mqtt_server_ip";
const int mqtt_port = 1883;
const char* client_id = "temp_sensor_1-";
const char* temperature_topic = "temperature";

// NTP
const char* ntp_server = "pool.ntp.org";
const long utc_offset = 0; // In seconds

// Loop
const long tick_interval = 5; // In seconds

```

## PlatformIO

This project is structured to be used with [PlatformIO](https://platformio.org/).
Once imported use `pio run --target upload` to upload the code to the mcu and `pio device monitor --port /dev/ttyUSB0` to connect via `serial_port` to see logs.
