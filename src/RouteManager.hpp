#pragma once
#include "pch.h"

namespace RouteManager {
static std::string apiKey;
static httplib::Client cli{"api.openrouteservice.org"};
static httplib::Headers headers;

void InitAPI() {
  std::ifstream inputFile(".env");
  inputFile >> apiKey;
  headers = {{"Authorization", apiKey}};
}

std::string GetCoordinateString(float latitude, float longtitude) {
  return std::to_string(latitude) + "," + std::to_string(longtitude);
}

void GetRoute(float startLatitude, float startLongtitude, float endLatitude,
              float endLongtitude) {
  auto start = GetCoordinateString(startLatitude, startLongtitude);
  auto end = GetCoordinateString(endLatitude, endLongtitude);
  httplib::Params params{{"api_key", apiKey}, {"start", start}, {"end", end}};
  auto route = cli.Post("/v2/directions/foot-hiking", headers, params);
  std::cout << route->body;
  /*if (route) {
    std::cout << route->body << std::endl;
  }*/
}

} // namespace RouteManager