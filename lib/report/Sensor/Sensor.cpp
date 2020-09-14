
#include "Sensor.hpp"
#include <ArduinoJson.h>

const char* tempSensorTypeString = "temperature";
const char* humiditySensorTypeString = "humidity";
const char* invalidSensorTypeString = "invalid";

Sensor::Sensor(
  SensorType type,
  ReportCommons::Measurement_t measurement,
  ReportCommons::Unit_t unit,
  ReportCommons::DateTime_t timestamp
) :
  _type(type),
  _measurement(measurement),
  _unit(unit),
  _timestamp(timestamp)
  {};

Sensor::~Sensor() {
  free(_timestamp);
}

void Sensor::dump(JsonObject& jsonObject) const {
  const char* typeStr = _typeToString();

  jsonObject["type"] = typeStr;
  jsonObject["measurement"] = _measurement;
  jsonObject["unit"] = _unit;
  jsonObject["timestamp"] = (const char*) _timestamp;

}

const char* Sensor::_typeToString() const {
  const char* typeStr;

  switch(_type) {
    case SensorType::temperature:
      typeStr = tempSensorTypeString;
      break;
    case SensorType::humidity:
      typeStr = humiditySensorTypeString;
      break;
    default:
      typeStr = invalidSensorTypeString;
  }

  return typeStr;
}
