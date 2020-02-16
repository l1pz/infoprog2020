#pragma once
#include "pch.h"

#include "IO.hpp"
#include "Sight.h"

namespace fs = std::filesystem;

namespace SightsManager {
static std::vector<Sight> sights;

std::optional<std::reference_wrapper<Sight>> SearchID(const unsigned id) {
  auto result{std::find_if(sights.begin(), sights.end(),
                           [id](Sight &sight) { return sight.id == id; })};
  if (result == sights.end())
    return std::nullopt;
  return *result;
}

std::optional<std::vector<std::reference_wrapper<const Sight>>>
SearchCategory(std::string_view category) {
  std::vector<std::reference_wrapper<const Sight>> results;
  for (const auto &sight : sights) {
    if (sight.category == category)
      results.emplace_back(sight);
  }
  if (results.empty())
    return std::nullopt;
  return results;
}

std::optional<std::vector<std::reference_wrapper<const Sight>>>
SearchText(std::string_view text) {
  std::vector<std::reference_wrapper<const Sight>> results;

  for (const auto &sight : sights) {
    auto result = std::search(
        sight.name.begin(), sight.name.end(), text.begin(), text.end(),
        [](char a, char b) { return std::tolower(a) == std::tolower(b); });
    if (result != sight.name.end())
      results.emplace_back(sight);
  }

  if (results.empty())
    return std::nullopt;

  return results;
}

void Delete(Sight &sight) {
  auto it = std::find(sights.begin(), sights.end(), sight);
  sights.erase(it);
  for (; it != sights.end(); it++)
    (*it).id--;
}

void Add(const unsigned id, std::string_view name, const float longtitude,
         const float latitude, std::string_view category, const float avgTime,
         std::string_view description) {
  sights.emplace_back(
      Sight(id, name, longtitude, latitude, category, avgTime, description));
}

void Load(const std::string &fileName) {
  std::ifstream fileInput(fileName);
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
  } else {
    std::cout << "A latnivalok.csv hiányzik!" << std::endl;
    exit(-1);
  }
}

void Save(const std::string &fileName) {
  std::ofstream fileOutput("data/" + fileName);
  fileOutput << "azonosito;nev;hosszusag;szelesseg;kategoria;atlagos_ido;leiras"
             << std::endl;
  std::cout.precision(6);
  for (auto &sight : sights) {
    fileOutput << sight.id << ';' << sight.name << ';' << std::fixed
               << sight.longtitude << ';' << std::fixed << sight.latitude << ';'
               << sight.category << ';' << std::fixed << sight.avgTime << ';'
               << sight.description << std::endl;
  }
  fileOutput.close();
}

void AddImage(Sight &sight, const fs::path filePath) {
  try {
    if (fs::file_size(filePath) > 2000000) {
      std::cout << "A fájl mérete túl nagy!" << std::endl;
      exit(-1);
    }

    fs::path newPath{"data/images/" + std::to_string(sight.id) + "/"};
    if (!fs::is_directory(newPath)) {
      fs::create_directory(newPath);
    }

    fs::path newFilePath{newPath / filePath.filename()};
    sight.images.emplace_back(newFilePath);
    fs::copy_file(filePath, fs::current_path() / newFilePath,
                  fs::copy_options::overwrite_existing);
  } catch (fs::filesystem_error &e) {
    std::cout << e.what() << std::endl;
  }
}
} // namespace SightsManager
