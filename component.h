#pragma once
#include "gameObject.h"
namespace engine {
    // forward declaration
    class GameObject;

    class Component {
    public:
        GameObject* gameObject;
        virtual void update();
        virtual void draw();
        virtual void start();

        void destroy();

        Component(GameObject* gameObject);

        ~Component();
    };
}
