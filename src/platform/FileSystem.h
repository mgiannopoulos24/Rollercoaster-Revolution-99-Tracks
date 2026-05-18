#ifndef RR99_PLATFORM_FILESYSTEM_H
#define RR99_PLATFORM_FILESYSTEM_H

#include <string>
#include <vector>

namespace rr99 {

class FileSystem {
public:
    static std::string getAssetPath(const std::string& relativePath);
    static std::string getSavePath(const std::string& filename);
    static bool exists(const std::string& path);
    static bool createDirectory(const std::string& path);
    static std::string readFile(const std::string& path);
    static bool writeFile(const std::string& path, const std::string& content);
};

} // namespace rr99

#endif // RR99_PLATFORM_FILESYSTEM_H
