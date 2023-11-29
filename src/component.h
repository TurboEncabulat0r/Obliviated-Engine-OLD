#pragma once
#include "gameObject.h"
namespace engine {
    // forward declaration
    class GameObject;
    class Component;

    class Component {
    public:
        GameObject* gameObject;
        std::string name;
        virtual void update();
        virtual void draw();
        virtual void start();

        void destroy();

        Component(GameObject* gameObject, std::string name);

        ~Component();
    };
}
