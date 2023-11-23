#pragma once
#include "renderer.h"
#include "engine.h"
#include "component.h"
#include <string>
using Texture = LPDIRECT3DTEXTURE9;
namespace engine{
    // forward declaration 
    class Component;

    class GameObject{
    public:
        std::string name;
        float x = 0;
        float y = 0;
        float size = 0;
        Texture tex;
        Component *components[100] = { nullptr };

        void draw();

        GameObject(float x, float y, float size, std::string name, Texture tex);

        ~GameObject();

        template <typename T>
        T* addComponent();

        template <typename T>
        T* getComponent();

        void destroyComponent(Component* component);


        void destroy();

    };
}
