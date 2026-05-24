#include "ConfigLoader.h"
#include <fstream>
#include <sstream>

bool ConfigLoader::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream ss(line);
        std::string key;
        float value;
        if (std::getline(ss, key, '=') && (ss >> value)) {
            values_[key] = value;
        }
    }
    return true;
}

int ConfigLoader::getInt(const std::string& key, int fallback) const {
    auto it = values_.find(key);
    return (it != values_.end()) ? static_cast<int>(it->second) : fallback;
}

float ConfigLoader::getFloat(const std::string& key, float fallback) const {
    auto it = values_.find(key);
    return (it != values_.end()) ? it->second : fallback;
}