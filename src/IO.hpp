#pragma once
#include "pch.h"

#include "Sight.h"

namespace IO {
inline void ListElement(const Sight &sight, unsigned &number) {
  std::cout << std::setw(4) << number << ": " << sight.name
            << ", id:" << sight.id << ", lat:" << sight.latitude
            << ", long:" << sight.longtitude << std::endl;
  number++;
}

template <class T> inline void List(const T &sights) {
  unsigned number{1};
  for (auto &sight : sights) {
    ListElement(sight, number);
  }
}

template <class T> inline void ListRef(const T &sights) {
  unsigned number{1};
  for (auto &sight : sights) {
    ListElement(sight.get(), number);
  }
}
} // namespace IO