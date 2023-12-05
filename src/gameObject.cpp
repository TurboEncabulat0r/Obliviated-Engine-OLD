#include "gameObject.h"
#include "engine.h"
#include "component.h"
#include "TestComp.h"
namespace engine{
    void GameObject::draw() {
        if (this->texture.lpdMat != nullptr)
            drawTexturedSquare(x, -y, size, 0xffffff, this->texture.lpdMat);
    }

    GameObject::GameObject(float x, float y, float size, std::string name, Texture tex) {
        this->x = x;
        this->y = y;
        this->size = size;
        this->texture = tex;
        this->name = name;
        registerGameObject(this);

        for (int i = 0; i < 20; i++) {
            if (components[i] != nullptr) {
                components[i]->start();
            }
        }
    }

    void GameObject::drawSerialValues() {
        // loops through all components and calls drawSerialElements
        for (int i = 0; i < 20; i++) {
            if (components[i] != nullptr) {
                if (components[i]->name.length() < 2)
                    continue;
                if (ImGui::CollapsingHeader(components[i]->name.c_str())) {
                    components[i]->drawSerialElements();
                }
            }
        }

    }


    GameObject::~GameObject() {
        destroy();
    }

    void GameObject::updateComponents() {
        for (int i = 0; i < 20; i++) {
            if (components[i] != nullptr) {
                components[i]->update();
            }
        }
    }

    void GameObject::update() {
        this->updateComponents();
        this->Update();
    }

    void GameObject::Update() {

    }

    void GameObject::destroy() {
        for (int i = 0; i < 20; i++) {
            if (components[i] != nullptr) {
                components[i]->destroy();
            }
        }
        unregisterGameObject(this);
    }

    //void GameObject::getComp(Component* c) {
    //    for (int i = 0; i < 20; i++) {
    //        if (components[i] == nullptr) {
    //            components[i] = c;
    //            break;
    //        }
    //    }
    //}
    
    template <typename T> T* GameObject::getComponent() {
        for (int i = 0; i < 20; i++) {
            if (components[i] != nullptr) {
                T* t = dynamic_cast<T*>(components[i]);
                if (t != nullptr) {
                    return t;
                }
            }
        }
        return nullptr;
    }

    void GameObject::addComp(Component* c) {
        for (int i = 0; i < 20; i++) {
            if (components[i] == nullptr) {
                components[i] = c;
                break;
            }
        }
    }

    void GameObject::destroyComponent(Component* component) {
        for (int i = 0; i < 20; i++) {
            if (components[i] == component) {
                components[i] = nullptr;
                delete component;
                return;
            }
        }
    }


}
