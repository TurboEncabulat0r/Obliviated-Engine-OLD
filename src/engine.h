#pragma once
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include <iostream>
#include <D3dx9tex.h>
#include <DirectXMath.h>

// components
#include "GameObject.h"
#include "camera.h"

using fn = void(void);
using fnc = void(char);

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

namespace engine {

    class GameObject;
    
    void onRender(fn* f);

    void onInit(fn*);

    void onDrawUI(fn*);

    void onUpdate(fn* f);

    void invokeOnRender();

    void registerButton(char*, bool*);

    void invokeOnDrawUi();

    void invokeOnUpdate();

    void invokeOnRender();

    bool getVsync();

    float getDeltaTime();

    void logf(char* str);
    void logf(char* str, char type);

    void registerGameObject(GameObject* go);

    void unregisterGameObject(GameObject* go);

    int FullRender();

    void ShutDown();
}
