#include <chrono>
#include "engine.h"
#include "game.h"
#include "renderer.h"
#include "input.h"



bool CreateDeviceD3D(HWND hWnd);

int programLoop()
{
    engine::logf("Starting main loop...");
    
    bool done = false;
    while (!done)
    {
        // handle win32 messages
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;

            if (msg.message == WM_KEYDOWN) {
                input::HandleKeyEvent((char)msg.wParam, true);
            }

            if (msg.message == WM_KEYUP) {
                input::HandleKeyEvent((char)msg.wParam, false);
            }
        }

        // quit fo we do any rendering and such
        if (done)
            break;

        engine::FullRender();
    }

    engine::ShutDown();
    return 0;
}

int main(int, char**)
{
    engine::setup();
    input::init();
    game::init();
    return programLoop();
}
