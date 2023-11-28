#pragma once
#include "engine.h"

namespace input {

    void HandleKeyEvent(char k, bool down);

    void onKeyPress(fnc* f);

    bool getKeyDown(char k);

    void init();
}

