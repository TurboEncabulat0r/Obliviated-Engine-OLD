#include <windows.h>
#include "input.h"
#include "imgui.h"
#include <iostream>
#define KEYBUFFERSIZE 12

namespace input {

    fnc* keySubscribers[10] = {};
    int keyIndex = 0;
    void onKeyPress(fnc* f) {
        keySubscribers[keyIndex] = f;
        keyIndex++;

    }

    void invokeOnKeyPress(char c) {
        for (int i = 0; i < 10; i++) {
            if (keySubscribers[i] != nullptr)
                keySubscribers[i](c);
        }
    }

    char keysDown[KEYBUFFERSIZE] = {};
    

    bool shiftPressed;
    void HandleKeyEvent(char k, bool down) {
        if (down) {
            if (k == VK_SHIFT) {
                shiftPressed = true;
            }
            invokeOnKeyPress(k);

            for (int i = 0; i < KEYBUFFERSIZE; i++) {
                if (keysDown[i] != k) {
                    if (keysDown[i] == 0) {
                        keysDown[i] = k;
                        break;
                    }
                }
                else {
                    break;
                }
            }

            return;
        }

        if (k == VK_SHIFT) {
            shiftPressed = false;

        }

        for (int i = 0; i < KEYBUFFERSIZE; i++) {
            if (keysDown[i] == k) {
                keysDown[i] = 0;
            }
        }

    }

    bool isOpen;
    int* current;
    void onDrawUi() {

        if (isOpen) {
            ImGui::Begin("Input Manager");

            ImGui::Text("Keys Down: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", keysDown);

            ImGui::Text("Shift Pressed: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", shiftPressed ? "true" : "false");

            ImGui::End();
        }

    }

    bool getKeyDown(char k) {
        for (int i = 0; i < KEYBUFFERSIZE; i++) {
            if (keysDown[i] != 0) {
                if (keysDown[i] == k) {
                    return true;
                }
            }
        }
        return false;
    }


    void init() {
        engine::logf("Input Manager init");
        ZeroMemory(keysDown, sizeof(keysDown));
        engine::onDrawUI(*onDrawUi);
        engine::registerButton("Input Manager", &isOpen);
    }




}

