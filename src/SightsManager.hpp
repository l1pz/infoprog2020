#pragma once
#include "pch.h"

#include "IO.hpp"
#include "Sight.h"

namespace fs = std::filesystem;

namespace SightsManager {
static std::vector<std::shared_ptr<Sight>> sights;
static std::unordered_set<std::string> categories;

std::optional<std::shared_ptr<Sight>> SearchID(const unsigned id) {
  auto result{std::find_if(
      sights.begin(), sights.end(),
      [id](std::shared_ptr<Sight> sight) { return sight->id == id; })};
  if (result == sights.end())
    return std::nullopt;
  return *result;
}

std::optional<std::vector<std::shared_ptr<const Sight>>>
SearchCategory(std::string_view category) {
  std::vector<std::shared_ptr<const Sight>> results;
  for (const auto &sight : sights) {
    if (sight->category == category)
      results.emplace_back(sight);
  }
  if (results.empty())
    return std::nullopt;
  return results;
}

std::optional<std::vector<std::shared_ptr<const Sight>>>
SearchText(std::string_view text) {
  std::vector<std::shared_ptr<const Sight>> results;

  for (const auto &sight : sights) {
    auto result = std::search(
        sight->name.begin(), sight->name.end(), text.begin(), text.end(),
        [](char a, char b) { return std::tolower(a) == std::tolower(b); });
    if (result != sight->name.end())
      results.emplace_back(sight);
  }

  if (results.empty())
    return std::nullopt;

  return results;
}

void Delete(const Sight &sight) {
  auto it = std::find_if(sights.begin(), sights.end(),
                         [sight](std::shared_ptr<Sight> otherSight) {
                           return otherSight->id == sight.id;
                         });
  assert(it != sights.end());
  fs::remove_all("data/images/" + std::to_string((*it)->id) + "/");
  it = sights.erase(it);
  for (; it != sights.end(); it++) {
    fs::path sightDirOld{"data/images/" + std::to_string((*it)->id) + "/"};
    fs::path sightDirNew{"data/images/" + std::to_string(--(*it)->id) + "/"};
    if (fs::is_directory(sightDirOld)) {
      fs::rename(sightDirOld, sightDirNew);
    }
  }
}

void Add(const unsigned id, std::string_view name, const float longtitude,
         const float latitude, std::string_view category, const float avgTime,
         std::string_view description) {
  sights.emplace_back(std::make_shared<Sight>(id, name, longtitude, latitude,
                                              category, avgTime, description));
}

inline void Clear() { sights.clear(); }

void PostLoad() {
  for (auto &sight : sights) {
    categories.insert(std::string(sight->category));
  }
}

void LoadCSV(const fs::path &filePath) {
  Clear();
  std::ifstream fileInput(filePath);
  if (fileInput) {
    bool header{true};
    std::string line;
    while (std::getline(fileInput, line)) {
      if (header) {
        header = false;
        continue;
      }
      if (line.empty())
        continue;
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ';', ' ');
      std::stringstream lineStream{line};
      int id;
      std::string name;
      float longtitude;
      float latitude;
      std::string category;
      float avgTime;
      std::string description;
      lineStream >> id >> name >> longtitude >> latitude >> category >>
          avgTime >> description;
      Add(id, name, longtitude, latitude, category, avgTime, description);
    }
    fileInput.close();
    PostLoad();
  } else {
    std::cout << "A latnivalok.csv hiányzik!" << std::endl;
  }
}

void SaveCSV(const fs::path &filePath) {
  std::ofstream fileOutput(filePath);
  fileOutput << "azonosito;nev;hosszusag;szelesseg;kategoria;atlagos_ido;leiras"
             << std::endl;
  std::cout.precision(6);
  for (auto &sight : sights) {
    fileOutput << sight->id << ';' << sight->name << ';' << std::fixed
               << sight->longtitude << ';' << std::fixed << sight->latitude
               << ';' << sight->category << ';' << std::fixed << sight->avgTime
               << ';' << sight->description << std::endl;
  }
  fileOutput.close();
}

void SaveBinary(const fs::path &filePath) {
  std::ofstream fileOutput(filePath, std::ios::binary);
  cereal::BinaryOutputArchive binaryArchive(fileOutput);
  binaryArchive(sights);
}

void LoadBinary(const fs::path &filePath) {
  Clear();
  std::ifstream fileInput(filePath, std::ios::binary);
  cereal::BinaryInputArchive binaryArchive(fileInput);
  binaryArchive(sights);
  PostLoad();
}

void SaveJSON(const fs::path &filePath) {
  std::ofstream fileOutput(filePath);
  cereal::JSONOutputArchive jsonArchive(fileOutput);
  jsonArchive(CEREAL_NVP(sights));
}

void LoadJSON(const fs::path &filePath) {
  Clear();
  std::ifstream fileInput(filePath);
  cereal::JSONInputArchive jsonArchive(fileInput);
  jsonArchive(sights);
  PostLoad();
}

void AddImage(Sight &sight, const fs::path &imagePath) {
  try {
    if (fs::file_size(imagePath) > 2000000) {
      std::cout << "A fájl mérete túl nagy!" << std::endl;
      exit(-1);
    }

    fs::path sightDir{"data/images/" + std::to_string(sight.id) + "/"};
    if (!fs::is_directory(sightDir)) {
      fs::create_directory(sightDir);
    }

    fs::path sightImagePath{sightDir / imagePath.filename()};
    sight.images.insert(sightImagePath.string());
    fs::copy_file(imagePath, fs::current_path() / sightImagePath,
                  fs::copy_options::overwrite_existing);
  } catch (fs::filesystem_error &e) {
    std::cout << e.what() << std::endl;
  }
}

void RemoveImage(Sight &sight, const fs::path &imagePath) {
  sight.images.erase(imagePath.string());
  fs::remove(imagePath);
}

} // namespace SightsManager
