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

std::string
CoordinatesArrayString(std::vector<std::pair<float, float>> &coordinates) {
  std::string arrayString;
  for (auto &coordinate : coordinates) {
    arrayString += '[' + std::to_string(coordinate.first) + ',' +
                   std::to_string(coordinate.second) + "],";
  }
  arrayString.pop_back();
  return arrayString;
}

void GetRoute(std::vector<std::pair<float, float>> &coordinates) {
  auto body =
      R"~(
    {"coordinates":[)~" +
      CoordinatesArrayString(coordinates) +
      R"~(),"instructions":"false","language":"hu","geometry":"false"})~";
  auto content = "application/json; charset=utf-8";
  auto route = cli.Post("/v2/directions/foot-hiking", headers, body, content);
  if (route && route->status == 200) {
    std::cout << route->body << std::endl;
  } else {
    std::cout << "Couldn't get route from the routing API.";
  }
}

double EstimateDistance(float lat1, float lon1, float lat2, float lon2) {
  auto R{6371e3}; // metres
  const auto rad{M_PI / 180};
  auto latRad1{lat1 * rad};
  auto latRad2{lat2 * rad};
  auto dLatRad{(lat2 - lat1) * rad};
  auto dLonRad{(lon2 - lon1) * rad};
  auto a = sin(dLatRad / 2) * sin(dLatRad / 2) +
           cos(latRad1) * cos(latRad2) * sin(dLonRad / 2) * sin(dLonRad / 2);
  auto c = 2 * atan2(sqrt(a), sqrt(1 - a));
  return R * c;
}

} // namespace RouteManager