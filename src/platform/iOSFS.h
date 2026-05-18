#ifndef RR99_PLATFORM_IOSFS_H
#define RR99_PLATFORM_IOSFS_H

#include <string>

namespace rr99 {

class iOSFS {
public:
    static void initialize();
    static std::string getDocumentsPath();
    static std::string readBundleAsset(const std::string& path);
    static void onBackground();
    static void onForeground();
};

} // namespace rr99

#endif // RR99_PLATFORM_IOSFS_H
