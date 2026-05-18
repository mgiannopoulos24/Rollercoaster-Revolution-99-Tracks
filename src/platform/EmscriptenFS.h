#ifndef RR99_PLATFORM_EMSCRIPTENFS_H
#define RR99_PLATFORM_EMSCRIPTENFS_H

#include <string>

namespace rr99 {

class EmscriptenFS {
public:
    static void syncToIndexedDB();
    static void resumeAudioContext();
};

} // namespace rr99

#endif // RR99_PLATFORM_EMSCRIPTENFS_H
