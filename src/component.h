#pragma once
#include "gameObject.h"
#include <vector>
namespace engine {
    // forward declaration
    class GameObject;
    class Component;

    struct serialValue {
        const char* label;
        char type = 0;
        uintptr_t ptr;
        serialValue(const char* l, char t, uintptr_t p) {
            this->label = l;
            this->type = t;
            this->ptr = p;
        }

        serialValue() {
            this->type = 0;
        }
    };

    class Component {
        std::vector<serialValue> values;
        int nValues = 10;
    public:
        GameObject* gameObject;
        std::string name;
        virtual void update();
        virtual void draw();
        virtual void start();

        void addSerialValue(const char* label, char t, uintptr_t p);



        void serializeValue(const char* tag, float* v);

        void serializeValue(const char* tag, int* v);

        void serializeValue(const char* tag, bool* v);

        void serializeValue(const char* tag, std::string* s);

        void drawSerialElements();

        void destroy();


        Component(GameObject* gameObject, std::string name);

        ~Component();
    };
}
