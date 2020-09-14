#include "WeatherReport.hpp"

WeatherReport::WeatherReport(
  ReportCommons::StationId_t stationId,
  ReportCommons::DateTime_t timestamp,
  Sensor** sensors,
  int sensorsAmount
) :
  Report(stationId, timestamp),
  _sensors(sensors),
  _sensorsAmount(sensorsAmount)
  {};


WeatherReport::~WeatherReport() {

  for(int i = 0; i < _sensorsAmount; i++) {
    delete _sensors[i];
  }

  free(_sensors);
}

void WeatherReport::dump(JsonObject& jsonObject) const {

  Report::dump(jsonObject);

  JsonArray sensorsJsonArray = jsonObject.createNestedArray("sensors");

  for(int i = 0; i < _sensorsAmount; i++) {
    JsonObject sensorJsonObject = sensorsJsonArray.createNestedObject();
    _sensors[i]->dump(sensorJsonObject);
  }

}
