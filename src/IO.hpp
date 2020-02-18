#pragma once
#include "pch.h"

#include "Sight.h"

namespace IO {
void ListElement(const Sight &sight, unsigned &number) {
  std::cout << std::setw(4) << number << ": " << sight.name
            << ", id:" << sight.id << ", lat:" << sight.latitude
            << ", long:" << sight.longtitude << ", avgTime:" << sight.avgTime
            << std::endl;
  number++;
}

void List(const std::vector<std::shared_ptr<Sight>> &sights) {
  unsigned number{1};
  for (auto &sight : sights) {
    ListElement(*sight, number);
  }
}
} // namespace IO