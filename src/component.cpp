#include "component.h"


namespace engine {
    Component::Component(GameObject* gameObject, std::string name) {
        this->gameObject = gameObject;
        registerComponent(this);
    }

    Component::~Component() {
        this->destroy();
    }

    void Component::destroy() {
        this->gameObject->destroyComponent(this);
    }

    void Component::update() {
        
    }

    void Component::draw() {

    }

    void Component::start() {

    }
}
