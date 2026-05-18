#ifndef RR99_PLATFORM_ANDROIDFS_H
#define RR99_PLATFORM_ANDROIDFS_H

#include <string>

namespace rr99 {

class AndroidFS {
public:
    static void initialize();
    static std::string getInternalStoragePath();
    static std::string readAsset(const std::string& path);
    static void onPause();
    static void onResume();
};

} // namespace rr99

#endif // RR99_PLATFORM_ANDROIDFS_H
