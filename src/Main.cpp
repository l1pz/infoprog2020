#include "pch.h"

#include "RouteManager.hpp"
#include "SightsManager.hpp"

using namespace SightsManager;
using namespace RouteManager;

int main() {
  LoadBinary("latnivalok.bin");
  auto route = GenerateRoute(sights);
  IO::List(route.first);
  std::cout << "Becsült idő: " << route.second << std::endl;
  std::vector<std::pair<float, float>> coordinates;
  for (auto sight : route.first) {
    coordinates.emplace_back(
        std::make_pair(sight->longtitude, sight->latitude));
  }
  InitAPI();
  GetRouteLenght(coordinates);
}