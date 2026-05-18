#include "EmscriptenFS.h"

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/html5.h>
    #include <iostream>

namespace rr99 {

void EmscriptenFS::syncToIndexedDB() {
    // In Emscripten, we call FS.syncfs to persist MEMFS to IDBFS
    EM_ASM({
        FS.syncfs(
            false, function(err) {
                if (err) {
                    console.error("FS.syncfs failed:", err);
                } else {
                    console.log("FS.syncfs completed successfully.");
                }
            });
    });
}

void EmscriptenFS::resumeAudioContext() {
    // Browsers require a user gesture to resume audio context
    EM_ASM({
        if (typeof AudioContext != = 'undefined') {
            var audioCtx = new (window.AudioContext || window.webkitAudioContext)();
            if (audioCtx.state == = 'suspended') {
                audioCtx.resume().then(() = > { console.log("AudioContext resumed."); });
            }
        }
    });
}

} // namespace rr99
#else
namespace rr99 {
void EmscriptenFS::syncToIndexedDB() {
}
void EmscriptenFS::resumeAudioContext() {
}
} // namespace rr99
#endif
