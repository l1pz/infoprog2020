#include "pch.h"

#include "SightsManager.hpp"

using namespace SightsManager;

int main() {
  Load("data/latnivalok.csv");
  IO::List(sights);
  std::cout << Sight::counter;
}