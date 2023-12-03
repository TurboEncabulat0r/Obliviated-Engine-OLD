#include "engine.h"
#include "renderer.h"
#include "gameObject.h"
#include <vector>
#include <chrono>
namespace engine {
    bool vsync = true;

    auto start = std::chrono::high_resolution_clock::now();

    void drawUi();
    void updateGameObjects();

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

    std::vector<fn*> updateSubs;
    void onUpdate(fn* f) {
        updateSubs.push_back(f);
    }

    void invokeOnDrawUi() {
        drawUi();
        for (int i = 0; i < 10; i++) {
            if (drawUiSubs[i] != nullptr)
                drawUiSubs[i]();
        }
    }

    void invokeOnUpdate() {
        updateGameObjects();
        for (int i = 0; i < updateSubs.size(); i++) {
            if (updateSubs[i] != nullptr)
                updateSubs[i]();
        }
    }

    std::vector<Component*> components;
    int regComponents = 0;
    void registerComponent(Component* c) {
        if (components.size() == 0 || regComponents + 1 >= components.size()) {
            components.resize(components.size() + 10);

        }
        for (int i = 0; i < components.size(); i++) {
            if (components[i] == nullptr) {
                components[i] = c;
                regComponents++;
                logf("Registered component");
                return;
            }
        }
    }



    std::vector<GameObject*> gameObjects;
    int registeredGameObjects = 0;
    //GameObject* gameObjects[100] = {};
    void renderGameObjects() {

        for (int i = 0; i < gameObjects.size(); i++) {
            if (gameObjects[i] != nullptr) {
                gameObjects[i]->draw();
            }
        }
    }

    void updateGameObjects() {
        for (int i = 0; i < gameObjects.size(); i++) {
            if (gameObjects[i] != nullptr) {
                gameObjects[i]->update();
            }
        }
    }

    void registerGameObject(GameObject* go) {
        if (gameObjects.size() == 0 || registeredGameObjects + 1 >= gameObjects.size()) {
            logf("alloc more gameobjects");
            gameObjects.resize(gameObjects.size() + 10);

        }
        for (int i = 0; i < gameObjects.size(); i++) {
            if (gameObjects[i] == nullptr) {
                gameObjects[i] = go;
                registeredGameObjects++;
                logf("Registered gameobject");
                return;
            }
        }
        logf("Failed to register gameobject", 'e');
    }

    void unregisterGameObject(GameObject* go) {
        for (int i = 0; i < gameObjects.size(); i++) {
            if (gameObjects[i] == go) {
                gameObjects[i] = nullptr;
                registeredGameObjects--;
                logf("Unregistered gameobject");
                return;
            }
        }
        logf("Failed to unregister gameobject", 'e');
    }


    void logf(const char* str, char type) {
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

    void logf(const char* str) {
        std::cout << "[INFO] " << str << std::endl;
    }

    void logf(std::string str) {
        std::cout << "[INFO] " << str << std::endl;
    }


    GameObject* selected;
    void drawHierarchy() {
        ImGui::Begin("Hierarchy");
        if (ImGui::Button("Create Empty")) {
            GameObject* go = new GameObject(0, 0, 0, "GameObject", Texture());
        }
        for (int i = 0; i < gameObjects.size(); i++) {
            if (gameObjects[i] != nullptr) {
                if (ImGui::Button(gameObjects[i]->name.c_str(), ImVec2(ImGui::GetWindowWidth(), 0))) {
                    selected = gameObjects[i];
                }
            }
        }
        ImGui::End();
    }


    void drawInspector() {
        ImGui::ShowDemoWindow();
        ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoResize);
        if (selected != nullptr) {
            ImGui::Text(selected->name.c_str());
            // editable values
            ImGui::SetNextItemWidth(140);
            ImGui::InputText("name", &selected->name[0], 100);
            ImGui::SetNextItemWidth(60);
            ImGui::DragFloat("x", &selected->x);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            ImGui::DragFloat("y", &selected->y);
            ImGui::SetNextItemWidth(140);
            ImGui::DragFloat("size", &selected->size);
            // dropdown selectbox for texture
            ImGui::SetNextItemWidth(140);

            // draws a dropdown selectbox for textures
            std::vector<Texture> textures;
            getRegisteredTextures(&textures);
            int selectedTexture = 0;
            for (int i = 0; i < textures.size(); i++) {
                if (textures[i].name == selected->texture.name || textures[i].lpdMat == selected->texture.lpdMat) {
                    selectedTexture = i;
                }
            }
            std::vector<const char*> textureNames;
            for (int i = 0; i < textures.size(); i++) {
                textureNames.push_back(textures[i].name.c_str());
            }
            ImGui::Combo("Texture", &selectedTexture, &textureNames[0], textureNames.size());
            selected->texture = textures[selectedTexture];




            

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

    float getTime() {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = now - start;
        return duration.count();
    }

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

        ImGui::Checkbox("VSync", &vsync);

        if (ImGui::Button("stats")) {
            stats = !stats;
        }

        //ImGui::ShowDemoWindow();

        // button for loading a texture, pops up a file dialog and loads the texture
        if (ImGui::Button("Load Texture")) {
            OPENFILENAME ofn;
            TCHAR szFile[MAX_PATH] = { 0 };
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = _T("Images\0*.bmp;*.png;*.jpg;*.jpeg;*.gif\0");
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrTitle = _T("Select an Image");
            ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

            if (GetOpenFileName(&ofn)) {
                LPCWSTR path = ofn.lpstrFile;

                std::wstring wpath(path);

                
                
                LPCWSTR lpath = wpath.c_str();
                Texture t;
                loadTexture("loaded", lpath, &t);
            }
        }


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

    void setup() {
        rendererInit();

        updateSubs.resize(10);

    }
}
