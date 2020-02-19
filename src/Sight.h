#pragma once

namespace fs = std::filesystem;

struct Sight : std::enable_shared_from_this<Sight> {
  // Functions
  Sight(const unsigned id, std::string_view name, const float longtitude,
        const float latitude, std::string_view category, const float avgTime,
        const std::string_view description);

  Sight() = default;

  inline bool operator<(const Sight &other) const {
    return this->id < other.id;
  }

  inline bool operator==(const Sight &other) const {
    return this->id == other.id;
  }

  template <class Archive> void serialize(Archive &archive) {
    archive(CEREAL_NVP(id), CEREAL_NVP(name), CEREAL_NVP(longtitude),
            CEREAL_NVP(latitude), CEREAL_NVP(category), CEREAL_NVP(avgTime),
            CEREAL_NVP(description), CEREAL_NVP(images));
  }

  friend nana::listbox::oresolver &operator<<(nana::listbox::oresolver &orr,
                                              const Sight &sight) {
    orr << sight.name;
    return orr;
  }

  friend std::ostream &operator<<(std::ostream &orr, const Sight &sight) {
    orr << sight.name;
    return orr;
  }

  friend nana::listbox::iresolver &operator>>(nana::listbox::iresolver &orr,
                                              Sight &sight) {
    orr >> sight.name;
    return orr;
  }

  // Variables
  unsigned id;
  std::string name;
  float longtitude;
  float latitude;
  std::string category;
  float avgTime;
  std::string description;
  std::unordered_set<std::string> images;
};
