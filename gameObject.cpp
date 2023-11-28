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

        ZeroMemory(this->values, sizeof(values));

        for (int i = 0; i < 20; i++) {
            if (components[i] != nullptr) {
                components[i]->start();
            }
        }
    }



    void GameObject::addSerialValue(const char* label, char t, uintptr_t p) {
        logf("serialized value ");
        for (int i = 0; i < nValues; i++) {
            if (values[i].type == 0) {
                values[i] = serialValue(label, t, p);
                break;
            }
        }

        nValues = (sizeof(this->values) / sizeof(serialValue));
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
        for (int i = 0; i < this->nValues; i++) {
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

    void GameObject::destroy() {
        for (int i = 0; i < 20; i++) {
            if (components[i] != nullptr) {
                components[i]->destroy();
            }
        }
        unregisterGameObject(this);
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
