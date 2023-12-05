#include "TestComp.h"


namespace engine {

    TestComp::TestComp(GameObject* gameObject) : Component(gameObject, "TEST") {
        this->serializeValue("Gaming", &this->gaming);
    }

    void TestComp::update() {
        gaming += 1;
    }

}
