#include "FileSystem.h"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace rr99 {

// This is a very basic implementation. In a real project, this would
// use platform-specific logic (Android, iOS, Emscripten, etc.)

std::string FileSystem::getAssetPath(const std::string& relativePath) {
    // For now, just assume assets are in the current working directory's assets/
    return "assets/" + relativePath;
}

std::string FileSystem::getSavePath(const std::string& filename) {
    // For now, just use a 'saves/' directory in the current working directory
    return "saves/" + filename;
}

bool FileSystem::exists(const std::string& path) {
    return std::filesystem::exists(path);
}

bool FileSystem::createDirectory(const std::string& path) {
    return std::filesystem::create_directories(path);
}

std::string FileSystem::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return "";
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool FileSystem::writeFile(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) return false;
    file << content;
    return true;
}

} // namespace rr99
