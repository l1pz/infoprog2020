#include "pch.h"

#include "RouteManager.hpp"
#include "SightsManager.hpp"

using namespace SightsManager;
using namespace RouteManager;

int main() {
  LoadBinary("latnivalok.bin");
  // IO::List(sights);
  // SaveBinary("latnivalok.bin");

  InitAPI();
  std::vector<std::pair<float, float>> coordinates;
  for (auto &sight : sights) {
    coordinates.push_back({std::make_pair(sight.latitude, sight.longtitude)});
  }
  std::cout << EstimateDistance(
                   coordinates.front().first, coordinates.front().second,
                   coordinates.back().first, coordinates.back().second)
            << std::endl;

  std::cout << coordinates.front().first << " " << coordinates.front().second
            << " " << coordinates.back().first << " "
            << coordinates.back().second << std::endl;
}