#pragma once
#include "pch.h"

namespace RouteManager {
static std::string apiKey;
static httplib::Client cli{"api.openrouteservice.org"};
static httplib::Headers headers;

void InitAPI() {
  std::ifstream inputFile(".env");
  inputFile >> apiKey;
  headers = {{"Accept", "application/json, application/geo+json, "
                        "application/gpx+xml, img/png; charset=utf-8"},
             {"Authorization", apiKey}};
}

std::string GetCoordinateString(float latitude, float longtitude) {
  return std::to_string(latitude) + "," + std::to_string(longtitude);
}

void GetRoute(float startLatitude, float startLongtitude, float endLatitude,
              float endLongtitude) {
  auto start = GetCoordinateString(startLatitude, startLongtitude);
  auto end = GetCoordinateString(endLatitude, endLongtitude);
  httplib::Params params{{"api_key", apiKey}, {"start", start}, {"end", end}};
  auto body = R"~(
    {"coordinates":[[8.681495,49.41461],[8.686507,49.41943],[8.687872,49.420318]],"instructions":"false","language":"hu","geometry":"false"}
    )~";
  auto content = "application/json; charset=utf-8";
  auto route = cli.Post("/v2/directions/foot-hiking", headers, body, content);
  std::cout << route->body;
  /*if (route) {
    std::cout << route->body << std::endl;
  }*/
}

} // namespace RouteManager