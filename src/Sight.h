#pragma once

namespace fs = std::filesystem;

struct Sight {
  // Functions
  Sight(const unsigned id, std::string_view name, const float longtitude,
        const float latitude, std::string_view category, const float avgTime,
        const std::string_view description);

  Sight() = default;
  inline bool operator<(const Sight &other) const {
    return (this->id < other.id);
  }
  inline bool operator==(const Sight &other) const {
    return this->id == other.id;
  }

  // Változók
  unsigned id;
  std::string name;
  float longtitude;
  float latitude;
  std::string category;
  float avgTime;
  std::string description;
  std::vector<fs::path> images;
};
