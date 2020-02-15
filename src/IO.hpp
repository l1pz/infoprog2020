#pragma once
#include "pch.h"

#include "Sight.h"

namespace IO {
template <class T> inline void List(T sights) {
  unsigned number{1};
  for (auto &sight : sights) {
    std::cout << std::setw(4) << number << ": " << sight->name << std::endl;
    number++;
  }
}
} // namespace IO