#include "pch.h"

#include "Sight.h"

Sight::Sight(const unsigned id_, std::string_view name_,
             const float longtitude_, const float latitude_,
             std::string_view category_, const float avgTime_,
             const std::string_view description_)
    : id(id_), name(name_), longtitude(longtitude_), category(category_),
      avgTime(avgTime_), description(description_) {
  std::replace(name.begin(), name.end(), '_', ' ');
  std::replace(category.begin(), category.end(), '_', ' ');
  std::replace(description.begin(), description.end(), '_', ' ');
}
