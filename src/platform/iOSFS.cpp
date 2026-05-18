#include "iOSFS.h"

namespace rr99 {

void iOSFS::initialize() {
    // Placeholder: on iOS, this would call Objective-C code to get
    // the app's Documents directory (NSFileManager).
}

std::string iOSFS::getDocumentsPath() {
    // Placeholder: real implementation would use NSSearchPathForDirectoriesInDomains
    return ".";
}

std::string iOSFS::readBundleAsset(const std::string& path) {
    // Placeholder: real implementation would read from the app bundle
    // using NSBundle.
    (void)path;
    return "";
}

void iOSFS::onBackground() {
    // Placeholder: save state, release Metal/OpenGL resources.
}

void iOSFS::onForeground() {
    // Placeholder: restore renderer context.
}

} // namespace rr99
