#pragma once
#include "pch.h"

#include "Sight.h"

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
  std::cout << coordinates.size() << std::endl;
  std::string arrayString;
  for (auto &coordinate : coordinates) {
    arrayString += '[' + std::to_string(coordinate.first) + ',' +
                   std::to_string(coordinate.second) + "],";
  }
  arrayString.pop_back();
  return arrayString;
}

void GetRouteLenght(std::vector<std::pair<float, float>> &coordinates) {
  auto coordinatesArray = CoordinatesArrayString(coordinates);
  std::cout << coordinatesArray << std::endl;
  auto body{
      R"~(
    {"coordinates":[)~" +
      coordinatesArray +
      R"~(],"instructions":"false","language":"hu","geometry":"false"})~"};
  auto content{"application/json; charset=utf-8"};
  auto route{cli.Post("/v2/directions/foot-hiking", headers, body, content)};
  if (route && route->status == 200) {
    std::cout << route->body << std::endl;
  } else {
    std::cout << "Getting the route was unsuccessfull." << std::endl;
    std::cout << route->body << std::endl;
  }
}

double EstimateDistance(const Sight &s1, const Sight &s2) {
  auto R{6371e3}; // metres
  const auto rad{M_PI / 180};
  auto latRad1{s1.latitude * rad};
  auto latRad2{s2.latitude * rad};
  auto dLatRad{(s2.latitude - s1.latitude) * rad};
  auto dLonRad{(s2.longtitude - s1.longtitude) * rad};
  auto a = sin(dLatRad / 2) * sin(dLatRad / 2) +
           cos(latRad1) * cos(latRad2) * sin(dLonRad / 2) * sin(dLonRad / 2);
  auto c = 2 * atan2(sqrt(a), sqrt(1 - a));
  return R * c;
}

inline float EstimateTime(const Sight &s1, const Sight &s2) {
  return EstimateDistance(s1, s2) / 4000.0f;
}

inline float EstimateTime(float distance) { return distance / 4000.0f; }

std::pair<std::vector<std::shared_ptr<Sight>>, float>
GenerateRoute(std::vector<std::shared_ptr<Sight>> sights) {
  auto sight{sights[rand() % sights.size()]};
  float estimatedTime{sight->avgTime};
  std::vector<std::shared_ptr<Sight>> routeSights{sight};
  do {
    auto newSight{sights[rand() % sights.size()]};
    sights.erase(std::find(sights.begin(), sights.end(), newSight));
    if (std::find(routeSights.begin(), routeSights.end(), newSight) !=
        routeSights.end())
      continue;
    auto newTime{estimatedTime + EstimateTime(*routeSights.back(), *newSight) +
                 newSight->avgTime};
    if (newTime > 12)
      continue;
    routeSights.push_back(newSight);
    estimatedTime = newTime;

  } while (estimatedTime < 6 && sights.size() > 0);
  return {routeSights, estimatedTime};
}

} // namespace RouteManager