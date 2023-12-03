#include "gameObject.h"
#include "engine.h"
#include "component.h"
#include "TestComp.h"
namespace engine{
    void GameObject::draw() {
        if (this->texture.lpdMat != nullptr)
            drawTexturedSquare(x, y, size, 0xffffff, this->texture.lpdMat);
    }

    GameObject::GameObject(float x, float y, float size, std::string name, Texture tex) {
        this->x = x;
        this->y = y;
        this->size = size;
        this->texture = tex;
        this->name = name;
        registerGameObject(this);
        this->values.resize(10);

        for (int i = 0; i < 20; i++) {
            if (components[i] != nullptr) {
                components[i]->start();
            }
        }
    }


    void GameObject::addSerialValue(const char* label, char t, uintptr_t p) {
        if (values.size() == 0 || nValues + 1 >= values.size()) {
            values.resize(values.size() + 10);
            logf("alloc more values; new value: " + std::to_string(values.size()));

        }

        std::string s = label;
        logf("serialized value: " + s);
        for (int i = 0; i < nValues; i++) {
            if (values[i].type == 0) {
                values[i] = serialValue(label, t, p);
                nValues++;
                break;
            }
        }
    }

    // reinterprate values
    // int* newPtr = reinterpret_cast<int*>(intptr);
    void GameObject::serializeValue(const char* tag, float* v) {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(v);

        addSerialValue(tag ,'f', ptr);
    }

    void GameObject::serializeValue(const char* tag, int* v) {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(v);

        addSerialValue(tag, 'i', ptr);
    }

    void GameObject::serializeValue(const char* tag, bool* v) {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(v);

        addSerialValue(tag, 'b', ptr);
    }

    void GameObject::serializeValue(const char* tag, std::string* v) {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(v);

        addSerialValue(tag, 's', ptr);
    }

    void drawSerialElement(serialValue v) {
        int* iptr;
        float* fptr;
        bool* bptr;
        switch (v.type)
        {
        case 'i':
            iptr = reinterpret_cast<int*>(v.ptr);
            ImGui::InputInt(v.label, iptr);
            break;
        case 'f':
            fptr = reinterpret_cast<float*>(v.ptr);
            ImGui::DragFloat(v.label, fptr);
            break;
        case 'b':
            bptr = reinterpret_cast<bool*>(v.ptr);
            ImGui::Checkbox(v.label, bptr);
            break;
        case 't':
            bptr = reinterpret_cast<bool*>(v.ptr);
            ImGui::Checkbox(v.label, bptr);
            break;
        default:
            break;
        }
    }

    void GameObject::drawSerialElements() {
        for (int i = 0; i < this->values.size(); i++) {
            if (this->values[i].type != 'n') {
                ImGui::SetNextItemWidth(140);
                drawSerialElement(this->values[i]);
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
