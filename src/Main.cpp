#include "pch.h"

#include "LatnivaloKezelo.hpp"

using namespace LatnivaloKezelo;

int main() {
  Betolt("data/latnivalok.csv");
  IO::Listaz(latnivalok);
}