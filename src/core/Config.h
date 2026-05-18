#ifndef RR99_CORE_CONFIG_H
#define RR99_CORE_CONFIG_H

#include <map>
#include <string>

namespace rr99 {

struct Keybinding {
    int keyboard_key;
    int gamepad_button;
};

class Config {
public:
    static Config& instance();

    void load();
    void save();

    void setInt(const std::string& key, int value);
    int getInt(const std::string& key, int defaultValue) const;

    void setFloat(const std::string& key, float value);
    float getFloat(const std::string& key, float defaultValue) const;

    void setBool(const std::string& key, bool value);
    bool getBool(const std::string& key, bool defaultValue) const;

    void setString(const std::string& key, const std::string& value);
    std::string getString(const std::string& key, const std::string& defaultValue) const;

    void setKeybinding(const std::string& key, Keybinding binding);
    Keybinding getKeybinding(const std::string& key, Keybinding defaultBinding) const;

private:
    Config() = default;
    std::string m_filePath;
    std::map<std::string, int> m_ints;
    std::map<std::string, float> m_floats;
    std::map<std::string, bool> m_bools;
    std::map<std::string, std::string> m_strings;
    std::map<std::string, Keybinding> m_keybindings;
};

} // namespace rr99

#endif // RR99_CORE_CONFIG_H
