#pragma once
#include "engine.h"
#include "camera.h"
#include <vector>
namespace engine {
    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    struct CUSTOMVERTEX
    {
        float x; /**< The x-coordinate of the vertex position. */
        float y; /**< The y-coordinate of the vertex position. */
        float z; /**< The z-coordinate of the vertex position. */
        float rhw; /**< The reciprocal of the homogeneous w-coordinate. */
        DWORD color; /**< The color of the vertex. */
    };

    struct TEXTUREDVERTEX
    {
        float x; /**< The x-coordinate of the vertex position. */
        float y; /**< The y-coordinate of the vertex position. */
        float z; /**< The z-coordinate of the vertex position. */
        float rhw; /**< The reciprocal of the homogeneous w-coordinate. */
        DWORD color; /**< The color of the vertex. */
        float tu; /**< The u-coordinate of the texture coordinate. */
        float tv; /**< The v-coordinate of the texture coordinate. */
    };

    class Texture {
    public:
        LPDIRECT3DTEXTURE9 lpdMat;
        int width;
        int height;
        std::string name;
        Texture(LPDIRECT3DTEXTURE9 tex, std::string name);
        Texture();
    };

    

    void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int color);

    void drawSquare(float x, float y, float size, int color);

    void drawTexturedSquare(float x, float y, float size, int color, LPDIRECT3DTEXTURE9 texture);

    void setup();

    void newFrame();

    void endFrame();

    int loadTexture(std::string name, LPCWSTR path, Texture* tex);

    void getRegisteredTextures(std::vector<Texture>* t);

    void getCamera(Camera2D** cam);

    void rendererInit();

    


}
