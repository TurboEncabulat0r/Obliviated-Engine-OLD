#pragma once
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include <iostream>
#include <D3dx9tex.h>
#include <DirectXMath.h>
#include "renderer.h"

// components
#include "gameObject.h"
#include "component.h"

using fn = void(void);
using fnc = void(char);

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

namespace engine {
    class GameObject;
    class Texture;
    class Component;
    
    void onRender(fn* f);

    void onInit(fn*);

    void onDrawUI(fn*);

    void onUpdate(fn* f);

    void invokeOnRender();

    void registerButton(char*, bool*);

    void invokeOnDrawUi();

    void invokeOnUpdate();

    void invokeOnRender();
    void registerTexture(Texture t);

    bool getVsync();

    float getDeltaTime();

    void logf(const char* str);
    void logf(std::string str);
    void logf(const char* str, char type);

    void registerGameObject(GameObject* go);

    void unregisterGameObject(GameObject* go);

    void registerComponent(Component* c);

    int FullRender();

    void ShutDown();

    float getTime();
}
