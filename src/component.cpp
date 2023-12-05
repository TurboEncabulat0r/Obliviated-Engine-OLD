#include "component.h"


namespace engine {
    Component::Component(GameObject* gameObject, std::string name) {
        this->gameObject = gameObject;
        this->name = name;
        registerComponent(this);
    }

    Component::~Component() {
        this->destroy();
    }

    void Component::addSerialValue(const char* label, char t, uintptr_t p) {
        if (values.size() == 0 || nValues + 1 >= values.size()) {
            values.resize(values.size() + 10);
            logf("alloc more values; new value: " + std::to_string(values.size()));

        }

        std::string s = label;
        
        for (int i = 0; i < nValues; i++) {
            if (values[i].type == 0) {
                values[i] = serialValue(label, t, p);
                logf("serialized value: " + s);
                nValues++;
                break;
            }
        }
    }

    // reinterprate values
    // int* newPtr = reinterpret_cast<int*>(intptr);
    void Component::serializeValue(const char* tag, float* v) {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(v);

        addSerialValue(tag, 'f', ptr);
    }

    void Component::serializeValue(const char* tag, int* v) {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(v);

        addSerialValue(tag, 'i', ptr);
    }

    void Component::serializeValue(const char* tag, bool* v) {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(v);

        addSerialValue(tag, 'b', ptr);
    }

    void Component::serializeValue(const char* tag, std::string* v) {
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

    void Component::drawSerialElements() {

        for (int i = 0; i < this->values.size(); i++) {
            if (this->values[i].type != 'n') {

                ImGui::SetNextItemWidth(140);
                drawSerialElement(this->values[i]);

            }

        }
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
