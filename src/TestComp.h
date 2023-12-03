#pragma once
#include "component.h"
#include "gameObject.h"
#include <string>

namespace engine {

    class TestComp : public Component {
    public:
        int gaming = 0;
        TestComp(GameObject* gameObject, std::string name);

        virtual void update();

    };

}
