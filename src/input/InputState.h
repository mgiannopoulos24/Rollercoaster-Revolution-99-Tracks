#ifndef RR99_INPUT_INPUTSTATE_H
#define RR99_INPUT_INPUTSTATE_H

namespace rr99 {

struct InputState {
    bool accelerating = false;
    bool braking = false;
    bool pause_pressed = false;
    bool menu_up = false;
    bool menu_down = false;
    bool menu_confirm = false;
    bool menu_cancel = false;

    void reset() {
        accelerating = false;
        braking = false;
        pause_pressed = false;
        menu_up = false;
        menu_down = false;
        menu_confirm = false;
        menu_cancel = false;
    }
};

} // namespace rr99

#endif // RR99_INPUT_INPUTSTATE_H
