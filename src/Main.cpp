#include "pch.h"

#include "RouteManager.hpp"
#include "SightsManager.hpp"

using namespace SightsManager;
using namespace RouteManager;

int main() {
  LoadBinary("latnivalok.bin");
  IO::List(sights);
  SaveBinary("latnivalok.bin");

  InitAPI();
  GetRoute(19.846984, 48.144734, 19.849476, 48.140213);
}