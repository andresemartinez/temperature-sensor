#ifndef WEATHER_REPORT_HPP
#define WEATHER_REPORT_HPP

#include "../ReportCommons.hpp"
#include "../Report.hpp"
#include "../Sensor/Sensor.hpp"

class WeatherReport : public Report {
  private:
    Sensor** _sensors;
    int _sensorsAmount;

  public:
    WeatherReport(
      ReportCommons::StationId_t stationId,
      ReportCommons::DateTime_t timestamp,
      Sensor** sensors,
      int sensorsAmount
    );
    ~WeatherReport();

    void dump(JsonObject& jsonObject) const;

};

#endif
