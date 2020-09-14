
#include "Report.hpp"

Report::Report(
  ReportCommons::StationId_t stationId,
  ReportCommons::DateTime_t timestamp
) :
  _stationId(stationId),
  _timestamp(timestamp)
  {};

Report::~Report() {
  free(_timestamp);
}

ReportCommons::Json_t Report::toJson() const {
  DynamicJsonDocument jsonReportDocument(512);
  JsonObject jsonReportObject = jsonReportDocument.to<JsonObject>();

  dump(jsonReportObject);

  const size_t jsonSize = sizeof(char) *  512;
  ReportCommons::Json_t json = (ReportCommons::Json_t) malloc(jsonSize);

  serializeJson(jsonReportObject, json, jsonSize);

  return json;

}

void Report::dump(JsonObject& jsonObject) const {

  jsonObject["stationId"] = _stationId;
  jsonObject["timestamp"] = (const char*) _timestamp;

}
