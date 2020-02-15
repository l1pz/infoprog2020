#pragma once

struct Sight {
  // Functions
  Sight(const unsigned id, std::string_view name, const float longtitude,
        const float latitude, std::string_view category, const float avgTime,
        const std::string_view description);
  Sight() = default;
  inline Sight(const Sight &other) {}
  inline bool operator<(const Sight &other) const {
    return (this->id < other.id);
  }
  inline bool operator==(const Sight &other) const {
    return this->id == other.id;
  }

  // Változók
  static unsigned counter;
  unsigned id;
  std::string name;
  float longtitude;
  float latitude;
  std::string category;
  float avgTime;
  std::string description;
  std::vector<std::string> images;
};
