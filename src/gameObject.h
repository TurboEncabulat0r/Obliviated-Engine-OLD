#pragma once
#include "renderer.h"
#include "engine.h"
#include "component.h"
#include <string>
#include <vector>
using Texture = LPDIRECT3DTEXTURE9;
namespace engine{
    // forward declaration 
    class Component;



    class GameObject{


        void updateComponents();
        void addComp(Component* c);
    public:
        std::string name;
        float x = 0;
        float y = 0;
        float size = 0;
        Texture texture;
        Component *components[20] = { nullptr };

        void draw();

        void update();

        virtual void Update();

        GameObject(float x, float y, float size, std::string name, Texture tex);

        ~GameObject();

        void drawSerialValues();

        template <typename T> T* addComponent() {

            auto c = new T(this);
            addComp(c);
            return c;
        }

        template <typename T> T* getComponent();

        void destroyComponent(Component* component);


        void destroy();

    };
}
