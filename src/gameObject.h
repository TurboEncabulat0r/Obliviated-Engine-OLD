#pragma once
#include "renderer.h"
#include "engine.h"
#include "component.h"
#include <string>
#include <vector>
using Texture = LPDIRECT3DTEXTURE9;
namespace engine{
    // forward declaration 
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

    class GameObject{
        std::vector<serialValue> values;
        int nValues = 10;
    public:
        std::string name;
        float x = 0;
        float y = 0;
        float size = 0;
        Texture texture;
        Component *components[20] = { nullptr };

        void draw();

        GameObject(float x, float y, float size, std::string name, Texture tex);

        ~GameObject();

        void addSerialValue(const char* label, char t, uintptr_t p);

        void updateComponents();

        void serializeValue(const char* tag, float* v);

        void serializeValue(const char* tag, int* v);

        void serializeValue(const char* tag, bool* v);

        void serializeValue(const char* tag, std::string* s);

        void drawSerialElements();

        template <typename T>
        T* addComponent();

        template <typename T>
        T* getComponent();

        void destroyComponent(Component* component);


        void destroy();

    };
}
