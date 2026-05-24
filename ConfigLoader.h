#pragma once
#include <string>
#include <unordered_map>

class ConfigLoader {
public:
    bool loadFromFile(const std::string& filename);
    int getInt(const std::string& key, int fallback = 0) const;
    float getFloat(const std::string& key, float fallback = 0.f) const;

private:
    std::unordered_map<std::string, float> values_;
};