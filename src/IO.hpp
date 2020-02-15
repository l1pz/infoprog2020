#pragma once
#include "pch.h"

#include "Latnivalo.h"

namespace IO {
template <class T> inline void Listaz(T latnivalok) {
  unsigned sorszam{1};
  for (auto &latnivalo : latnivalok) {
    std::cout << std::setw(4) << sorszam << ": " << latnivalo->nev << std::endl;
    sorszam++;
  }
}
} // namespace IO