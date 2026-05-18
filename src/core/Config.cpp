#include "Config.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace rr99 {

Config& Config::instance() {
    static Config instance;
    return instance;
}

void Config::load() {
    // For now, we'll assume a default file path or handle it per platform.
    // This is a placeholder.
    std::ifstream file(m_filePath);
    if (!file.is_open())
        return;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string key, type, value;
        if (!(ss >> key >> type >> value))
            continue;

        if (type == "int")
            m_ints[key] = std::stoi(value);
        else if (type == "float")
            m_floats[key] = std::stof(value);
        else if (type == "bool")
            m_bools[key] = (value == "true");
        else if (type == "string")
            m_strings[key] = value;
    }
}

void Config::save() {
    std::ofstream file(m_filePath);
    if (!file.is_open())
        return;

    for (auto const& [key, val] : m_ints)
        file << key << " int " << val << "\n";
    for (auto const& [key, val] : m_floats)
        file << key << " float " << val << "\n";
    for (auto const& [key, val] : m_bools)
        file << key << " bool " << (val ? "true" : "false") << "\n";
    for (auto const& [key, val] : m_strings)
        file << key << " string " << val << "\n";
    // Keybindings are more complex, would need better serialization in real app
}

void Config::setInt(const std::string& key, int value) {
    m_ints[key] = value;
}
int Config::getInt(const std::string& key, int defaultValue) const {
    auto it = m_ints.find(key);
    return it != m_ints.end() ? it->second : defaultValue;
}

void Config::setFloat(const std::string& key, float value) {
    m_floats[key] = value;
}
float Config::getFloat(const std::string& key, float defaultValue) const {
    auto it = m_floats.find(key);
    return it != m_floats.end() ? it->second : defaultValue;
}

void Config::setBool(const std::string& key, bool value) {
    m_bools[key] = value;
}
bool Config::getBool(const std::string& key, bool defaultValue) const {
    auto it = m_bools.find(key);
    return it != m_bools.end() ? it->second : defaultValue;
}

void Config::setString(const std::string& key, const std::string& value) {
    m_strings[key] = value;
}
std::string Config::getString(const std::string& key, const std::string& defaultValue) const {
    auto it = m_strings.find(key);
    return it != m_strings.end() ? it->second : defaultValue;
}

void Config::setKeybinding(const std::string& key, Keybinding binding) {
    m_keybindings[key] = binding;
}
Keybinding Config::getKeybinding(const std::string& key, Keybinding defaultBinding) const {
    auto it = m_keybindings.find(key);
    return it != m_keybindings.end() ? it->second : defaultBinding;
}

} // namespace rr99
