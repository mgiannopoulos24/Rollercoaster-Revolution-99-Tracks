#include "AndroidFS.h"

namespace rr99 {

void AndroidFS::initialize() {
    // Placeholder: on Android, this would use JNI to get the activity's
    // getFilesDir() for internal storage and AAssetManager for asset reading.
}

std::string AndroidFS::getInternalStoragePath() {
    // Placeholder: return a default path; real implementation would
    // query JNI for the app's internal storage directory.
    return "/data/data/com.rollercoasterrevolution99/files/";
}

std::string AndroidFS::readAsset(const std::string& path) {
    // Placeholder: real implementation would use AAssetManager to read
    // assets from the APK.
    (void)path;
    return "";
}

void AndroidFS::onPause() {
    // Placeholder: save game state, release audio context, etc.
}

void AndroidFS::onResume() {
    // Placeholder: restore audio context, reload textures if needed.
}

} // namespace rr99
