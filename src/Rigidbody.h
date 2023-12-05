#pragma once
#include "component.h"
#include "gameObject.h"
#include <string>

namespace engine {

    class Rigidbody : public Component {
    public:
        float* x;
        float* y;
        float velx = 0;
        float vely = 0;
        float friction = 0.89;
        bool grounded;

        Rigidbody(GameObject* gameObject);

        virtual void update();

        void addVelocity(float x, float y);
    };

}
