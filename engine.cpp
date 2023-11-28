#include "engine.h"
#include "gameObject.h"

namespace engine {
    bool vsync = true;

    void drawUi();

    bool getVsync() {
        return vsync;
    }




    fn* renderSubscribers[10] = {};
    int renderIndex = 0;
    void onRender(fn* f) {
        renderSubscribers[renderIndex] = f;
        renderIndex++;

    }


    fn* drawUiSubs[10] = {};
    int drawUiIndex = 0;
    void onDrawUI(fn* f) {
        drawUiSubs[drawUiIndex] = f;
        drawUiIndex++;
    }


    fn* onUpdateSubs[10] = {};
    int onUpdateIndex = 0;
    void onUpdate(fn* f) {
        onUpdateSubs[onUpdateIndex] = f;
        onUpdateIndex++;
    }

    void invokeOnDrawUi() {
        drawUi();
        for (int i = 0; i < 10; i++) {
            if (drawUiSubs[i] != nullptr)
                drawUiSubs[i]();
        }
    }

    void invokeOnUpdate() {
        for (int i = 0; i < 10; i++) {
            if (onUpdateSubs[i] != nullptr)
                onUpdateSubs[i]();
        }
    }
    GameObject* gameObjects[100] = {};
    void renderGameObjects() {
        for (int i = 0; i < 100; i++) {
            if (gameObjects[i] != nullptr) {
                gameObjects[i]->draw();
            }
        }
    }

    void registerGameObject(GameObject* go) {
        for (int i = 0; i < 100; i++) {
            if (gameObjects[i] == nullptr) {
                gameObjects[i] = go;
                logf("Registered gameobject");
                return;
            }
        }
        logf("Failed to register gameobject", 'e');
    }

    void unregisterGameObject(GameObject* go) {
        for (int i = 0; i < 100; i++) {
            if (gameObjects[i] == go) {
                gameObjects[i] = nullptr;
                logf("Unregistered gameobject");
                return;
            }
        }
        logf("Failed to unregister gameobject", 'e');
    }


    void logf(char* str, char type) {
        if (type == 'd') {
            std::cout << "[DEBUG] " << str << std::endl;
        }
        else if (type == 'i') {
            std::cout << "[INFO] " << str << std::endl;
        }
        else if (type == 'e') {
            std::cout << "[ERROR] " << str << std::endl;
        }
    }

    void logf(char* str) {
        std::cout << "[INFO] " << str << std::endl;
    }


    GameObject* selected;
    void drawHierarchy() {
        ImGui::Begin("Hierarchy");
        if (ImGui::Button("Create Empty")) {
            GameObject* go = new GameObject(0, 0, 0, "GameObject", NULL);
        }
        for (int i = 0; i < 100; i++) {
            if (gameObjects[i] != nullptr) {
                if (ImGui::Button(gameObjects[i]->name.c_str(), ImVec2(ImGui::GetWindowWidth(), 0))) {
                    selected = gameObjects[i];
                }
            }
        }
        ImGui::End();
    }


    void drawInspector() {
        ImGui::Begin("Inspector");
        if (selected != nullptr) {
            ImGui::Text(selected->name.c_str());
            // editable values
            ImGui::InputFloat("x", &selected->x);
            ImGui::InputFloat("y", &selected->y);
            ImGui::InputFloat("size", &selected->size);
            ImGui::InputText("name", &selected->name[0], 100);
            selected->drawSerialElements();
        }
        ImGui::End();
    }

    void invokeOnRender() {
       
        renderGameObjects();
        for (int i = 0; i < 10; i++) {
            if (renderSubscribers[i] != nullptr)
                renderSubscribers[i]();
        }
    }



    struct button {
        char* name;
        bool* callback;
    };

    button* menuButtons[10] = {};
    int buttonindex = 0;

    void registerButton(char* name, bool* callback) {
        if (buttonindex >= 10)
            return;

        button* b = new button();
        b->name = name;
        b->callback = callback;

        menuButtons[buttonindex] = b;
        buttonindex++;
    }

    bool stats;
    void drawUi() {
        drawHierarchy();
        drawInspector();
        ImGui::Begin("Obliviated Engine");
        ImGui::Text("dasjkdj");

        ImGui::Checkbox("VSync", &vsync);

        if (ImGui::Button("stats")) {
            stats = !stats;
        }

        //ImGui::ShowDemoWindow();


        for (int i = 0; i < 10; i++) {
            if (menuButtons[i] != nullptr) {
                if (ImGui::Button(menuButtons[i]->name)) {
                    bool value = &menuButtons[i]->callback;
                    *menuButtons[i]->callback = !*menuButtons[i]->callback;
                }
            }
        }

        ImGui::End();

        if (stats) {
            ImGui::Begin("Stats");

            ImGui::Text("FPS: %f", 1.0f / getDeltaTime());
            ImGui::End();
        }

    }
}
