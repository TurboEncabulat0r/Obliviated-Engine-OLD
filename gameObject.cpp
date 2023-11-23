#include "engine.h"
#include "gameObject.h"
namespace engine{
    void GameObject::draw() {
        drawTexturedSquare(x, y, size, 0xffffff, tex);
    }

    GameObject::GameObject(float x, float y, float size, std::string name, Texture tex) {
        this->x = x;
        this->y = y;
        this->size = size;
        this->tex = tex;
        this->name = name;
        registerGameObject(this);
    }

    GameObject::~GameObject() {
        destroy();
    }

    void GameObject::destroy() {
        for (int i = 0; i < 100; i++) {
            if (components[i] != nullptr) {
                components[i]->destroy();
            }
        }
        unregisterGameObject(this);
    }

    void GameObject::destroyComponent(Component* component) {
        for (int i = 0; i < 100; i++) {
            if (components[i] == component) {
                components[i] = nullptr;
                delete component;
                return;
            }
        }
    }


}
