# Temperature Sensor

Temperature sensor made with an ESP8266 and a DTH22 developed with the Arduino framework.\
**Note**: This project was made as a hobby and is not tested for professional use.

## Config (config.hpp)

```c++
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
```

## MQTT Payload example

```json
{
    "stationId":"weather-station-1",
    "timestamp":"2020-08-13T23:02:49Z",
    "sensors":[
        {
            "type":"temperature",
            "measurement":24.6,
            "unit":"C",
            "timestamp":"2020-08-13T23:02:24Z"
        },
        {
            "type":"humidity",
            "measurement":50.1,
            "unit":"%",
            "timestamp":"2020-08-13T23:02:24Z"
        }
    ]
}
```

## PlatformIO

This project is structured to be used with [PlatformIO](https://platformio.org/).
Once imported use `pio run --target upload` to upload the code to the mcu and `pio device monitor --port /dev/ttyUSB0` to connect via `serial_port` to see logs.
