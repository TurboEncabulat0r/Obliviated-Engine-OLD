#include "engine.h"
#include "camera.h"
#include "input.h"
#include <DirectXMath.h>
#include <D3dx9math.h>
#include <sstream>
namespace engine {

    Camera2D::Camera2D(int screenWidth, int screenHeight, float angle, float scale) {
        //Update();

        this->position = DirectX::XMFLOAT3(0, 0, 0);

        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
        this->angle = angle;
        this->scale = scale;

        this->minScale = 0.15f;
        


        D3DXMatrixOrthoLH(&this->orthographicMatrix, screenWidth, -screenHeight, 0.0f, 1.0f);
        D3DXMatrixIdentity(&identityMatrix);
        std::stringstream ss1;
        logf("Camera initialized");
    }

    int camerax, cameray;
    void Camera2D::Update() {
        if (!this->isInitialized())
            return;

        if (input::getKeyDown(VK_RIGHT))
            this->position.x += -3;
        if (input::getKeyDown(VK_LEFT))
            this->position.x += 3;
        if (input::getKeyDown(VK_UP))
            this->position.y += 3;
        if (input::getKeyDown(VK_DOWN))
            this->position.y += -3;

        if (input::getKeyDown('P'))
            this->scale += 0.01;
        if (input::getKeyDown('O'))
            this->scale -= 0.01;

        if (this->scale <= minScale) {
            scale = minScale;
        }

        camerax = position.x;
        cameray = position.y;
       

        this->viewMatrix = D3DXMATRIX(
            scale * cos(angle), scale * sin(angle), 0, 0,
            -scale * sin(angle), scale * cos(angle), 0, 0,
            0, 0, 1, 0,
            -camerax * scale * cos(angle) + cameray * scale * sin(angle) + position.x,
            -camerax * scale * sin(angle) - cameray * scale * cos(angle) + position.y, 0, 1
        );
        

    }

    void Camera2D::SetTransform(LPDIRECT3DDEVICE9 dev) {
        if (!this->isInitialized())
            return;
        D3DXMATRIX worldViewProj = identityMatrix * viewMatrix * orthographicMatrix;
        dev->SetTransform(D3DTS_WORLD, &identityMatrix);
        dev->SetTransform(D3DTS_VIEW, &viewMatrix);
        dev->SetTransform(D3DTS_PROJECTION, &orthographicMatrix);

    }

    void Camera2D::move(float x, float y) {
        this->position.x += x;
        this->position.y += y;
        std::cout << this->position.x << std::endl;
    }

    bool Camera2D::isInitialized() {
        if (this != nullptr)
            return this->screenWidth != 0 && this->screenHeight != 0;
        logf("Main camera uninitialized!");
        return false;

    }

    void Camera2D::drawUi() {
        ImGui::Begin("Camera");
        ImGui::Text("Camera position: %f, %f", this->position.x, this->position.y);
        ImGui::Text("Camera scale: %f, %f", this->scale, this->scale);
        ImGui::End();
    }
}
