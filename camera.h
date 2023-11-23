#pragma once
#include <D3dx9tex.h>
#include <DirectXMath.h>
namespace engine {
    class Camera {
        D3DXVECTOR3 position;
        D3DXVECTOR3 lookAt;
        D3DXVECTOR3 up;

        D3DXMATRIX viewMatrix;
        D3DXMATRIX projectionMatrix;

        float fov; // field of view
        float aspectRatio;
        float nearClip;
        float farClip;

        Camera(float fov, float aspectRatio, float nearClip, float farClip);

        void Update();
    };


    class Camera2D {
    public:
        float angle;
        float scale;
        DirectX::XMFLOAT3 position;
        float minScale;

        D3DXMATRIX orthographicMatrix;
        D3DXMATRIX identityMatrix;
        D3DXMATRIX viewMatrix;



        float screenWidth;  // Width of the screen or viewport
        float screenHeight; // Height of the screen or viewport

        Camera2D(int screenWidth, int screenHeight, float angle, float scale);

        void Update();
        void move(float x, float y);
        bool isInitialized();
        void SetTransform(LPDIRECT3DDEVICE9 dev);
        void drawUi();
    };
}
