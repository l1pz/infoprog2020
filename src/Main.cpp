#include "pch.h"

#include "SightsManager.hpp"

using namespace SightsManager;

int main() {
  Load("data/latnivalok.csv");
  AddImage(SearchID(1).value(),
           "/home/l1pz/Pictures/Screenshot from 2020-01-28 14-50-16.png");
}