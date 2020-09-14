#ifndef REPORT_HPP
#define REPORT_HPP

#include "ReportCommons.hpp"

class Report {
  private:
    ReportCommons::StationId_t _stationId;
    ReportCommons::DateTime_t _timestamp;

  public:
    Report(
      ReportCommons::StationId_t stationId,
      ReportCommons::DateTime_t timestamp
    );
    virtual ~Report() = 0;

  ReportCommons::Json_t toJson() const;
  virtual void dump(JsonObject& jsonObject) const  = 0;

};

#endif
