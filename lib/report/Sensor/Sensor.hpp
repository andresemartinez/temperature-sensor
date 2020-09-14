
#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "../ReportCommons.hpp"

enum SensorType { temperature, humidity };

class Sensor {
  private:
    SensorType _type;
    ReportCommons::Measurement_t _measurement;
    ReportCommons::Unit_t _unit;
    ReportCommons::DateTime_t _timestamp;

    const char* _typeToString() const;

  public:
    Sensor(
      SensorType type,
      ReportCommons::Measurement_t measurement,
      ReportCommons::Unit_t unit,
      ReportCommons::DateTime_t timestamp
    );
    ~Sensor();

    void dump(JsonObject& jsonObject) const;

};

#endif
