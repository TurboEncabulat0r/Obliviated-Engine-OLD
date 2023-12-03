#include "TestComp.h"


namespace engine {

    TestComp::TestComp(GameObject* gameObject, std::string name) : Component(gameObject, name) {
        this->gameObject->serializeValue("Gaming", &this->gaming);
    }

    void TestComp::update() {
        gaming += 1;
    }

}
