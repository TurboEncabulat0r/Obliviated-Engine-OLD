#include "renderer.h"
#include <d3d9.h>
#include "imgui.h"
#include "engine.h"
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

#define SCREENWIDTH 1920
#define SCREENHEIGHT 1080

#define RENDERWIDTH 1920
#define RENDERHEIGHT 1080

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


namespace engine {

    float deltaTime = 0;
    static LPDIRECT3D9              g_pD3D = NULL;
    static LPDIRECT3DDEVICE9        device = NULL;
    static D3DPRESENT_PARAMETERS    g_d3dpp = {};
    IDirect3DSurface9* pRenderTargetSurface = nullptr;
    IDirect3DTexture9* pRenderTargetTexture = nullptr;
    IDirect3DSurface9* pBackBufferSurface = nullptr;

    Texture tex;

    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    void ResetDevice();

    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    WNDCLASSEXW wc;
    HWND hwnd;

    Camera2D* mainCamera;
   
    LPDIRECT3DVERTEXBUFFER9 v_buffer;

    std::vector<Texture> textures;
    int regTextures = 0;

    int InitializeWindow();


    void drawLine(float x1, float y1, float x2, float y2, int color){
        D3DXVECTOR2 lines[] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2) };
        D3DCOLOR colors[] = { color, color };
        device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

        // applies the 2d cameras translation from the view matrix
        for (int i = 0; i < 2; ++i) {
            lines[i].x += mainCamera->position.x;
            lines[i].y += mainCamera->position.y;
        }

        // applies the 2d cameras scale from the projection matrix
        for (int i = 0; i < 2; ++i) {
            lines[i].x *= mainCamera->scale;
            lines[i].y *= mainCamera->scale;
        }

        device->DrawPrimitiveUP(D3DPT_LINELIST, 1, lines, sizeof(D3DXVECTOR2));
    }

    void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int color) {
        D3DXVECTOR2 lines[] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2), D3DXVECTOR2(x3, y3) };
        D3DCOLOR colors[] = { color, color, color };
        device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

        // lock, copy, and unlock vertex buffer
        VOID* pVoid;
        v_buffer->Lock(0, 0, (void**)&pVoid, 0);
        memcpy(pVoid, lines, sizeof(lines));
        v_buffer->Unlock();

        device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, lines, sizeof(D3DXVECTOR2));
    }


    void RenderGrid()
    {
        const int gridSize = 10;  // Number of grid lines in each direction
        const float gridSpacing = 1.0f;  // Distance between grid lines

        struct Vertex
        {
            float x, y, z;  // Position of vertex in 3D space
            DWORD color;    // Diffuse color of vertex
        };

        Vertex vertices[(gridSize + 1) * 4];
        for (int i = 0; i <= gridSize; ++i)
        {
            float fPercent = i / (float)gridSize;
            float fPos = (fPercent * 2.0f) - 1.0f;
            DWORD col = (i == gridSize / 2) ? 0xffffffff : 0xff808080;
            vertices[i * 4 + 0] = { fPos, -1.0f, 0.0f, col };
            vertices[i * 4 + 1] = { fPos, 1.0f, 0.0f, col };
            vertices[i * 4 + 2] = { -1.0f, fPos, 0.0f, col };
            vertices[i * 4 + 3] = { 1.0f, fPos, 0.0f, col };
        }



        // render it
        device->SetRenderState(D3DRS_ZENABLE, FALSE);
        device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

        // transform the vertexes
        mainCamera->SetTransform(device);

        // multiplies scale of grid by 10
        for (int i = 0; i < gridSize * 4; ++i) {
            vertices[i].x *= 100;
            vertices[i].y *= 100;
        }


        device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
        device->DrawPrimitiveUP(D3DPT_LINELIST, gridSize * 4, vertices, sizeof(Vertex));

    }
    
    void drawTexturedSquare(float x, float y, float size, int color, LPDIRECT3DTEXTURE9 texture) {
        float halfSize = size * 0.5f;

        x *= 10;
        y *= 10;

        if (texture == nullptr) {
            return;
        }

        TEXTUREDVERTEX vertices[] = {
            { x - halfSize, y - halfSize, 0.5f, 1.0f, D3DCOLOR_ARGB(128, 255, 255, 255), 0.0f, 0.0f },
            { x + halfSize, y - halfSize, 0.5f, 1.0f, D3DCOLOR_ARGB(128, 255, 255, 255), 1.0f, 0.0f },
            { x - halfSize, y + halfSize, 0.5f, 1.0f, D3DCOLOR_ARGB(128, 255, 255, 255), 0.0f, 1.0f },
            { x + halfSize, y + halfSize, 0.5f, 1.0f, D3DCOLOR_ARGB(128, 255, 255, 255), 1.0f, 1.0f },
        };

        // applies the 2d cameras translation from the view matrix
        for (int i = 0; i < 4; ++i) {
            vertices[i].x += mainCamera->position.x;
            vertices[i].y += mainCamera->position.y;
        }

        // applies the 2d cameras scale from the projection matrix
        for (int i = 0; i < 4; ++i) {
            vertices[i].x *= mainCamera->scale;
            vertices[i].y *= mainCamera->scale;
        }


        // Lock, copy, and unlock vertex buffer
        VOID* pVoid;
        v_buffer->Lock(0, 0, (void**)&pVoid, 0);
        memcpy(pVoid, vertices, sizeof(vertices));
        v_buffer->Unlock();

        device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
        device->SetStreamSource(0, v_buffer, 0, sizeof(TEXTUREDVERTEX));

        // sets texture
        device->SetTexture(0, texture);
        device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    }

    void newFrame() {
        // setup vertex buffer for frame
        device->CreateVertexBuffer(8 * sizeof(TEXTUREDVERTEX),
        0, D3DFVF_XYZRHW | D3DFVF_DIFFUSE,
        D3DPOOL_MANAGED, &v_buffer, NULL);

        // sets the vertex buffer
        
    }

    void getRegisteredTextures(std::vector<Texture>* t) {
        std::vector<Texture> tex;

        for (int i = 0; i < textures.size(); i++) {
            if (textures[i].lpdMat != nullptr) {
                tex.push_back(textures[i]);
            }
        }

        *t = tex;
    }

    void registerTexture(Texture t) {
        if (textures.size() == 0 || regTextures + 1 >= textures.size()) {
            logf("alloc more textures");
            textures.resize(textures.size() + 10);

        }

        for (int i = 0; i < textures.size(); i++) {
            if (textures[i].lpdMat == nullptr) {
                textures[i] = t;
                logf("Registered texture");
                return;
            }
        }
        logf("Failed to register texture", 'e');
    }




    int loadTexture(std::string name, LPCWSTR path, Texture* tex) {
        //HRESULT res = D3DXCreateTextureFromFile(device, path, tex);

        //sets alpha from texture
        D3DXIMAGE_INFO info;
        Texture texture = Texture(NULL, name);
        D3DXGetImageInfoFromFile(path, &info);
        HRESULT res =D3DXCreateTextureFromFileEx(device, path, info.Width, info.Height, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, NULL, &texture.lpdMat);

        *tex = texture;

        engine::registerTexture(texture);

        if (res == D3D_OK) {
            std::string msg = "loaded texture " + name;
            logf(msg);
            return 1;
        }
        else {
            logf("Failed to create texture..", 'e');
            return 0;
        }
    }

    void endFrame() {
        // release the vertex buffer
        v_buffer->Release();
    }

    Texture::Texture(LPDIRECT3DTEXTURE9 tex, std::string name) {
        this->lpdMat = tex;
        this->name = name;
    }

    Texture::Texture() {
        this->lpdMat = nullptr;
        this->name = "Blank Texture";
        
    }


    // good function
    void getCamera(Camera2D** cam) {
        *cam = mainCamera;
    }

    void onDrawRendererUi() {
        mainCamera->drawUi();
    }

    float getDeltaTime() {
        return deltaTime;
    }


    void OnResize(int width, int height)
    {
        // Update the presentation parameters
        g_d3dpp.BackBufferWidth = width;
        g_d3dpp.BackBufferHeight = height;
        // Adjust the aspect ratio in the projection matrix
        D3DXMATRIX projectionMatrix;
        D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 4.0f, (float)width / (float)height, 1.0f, 1000.0f);
        device->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

        // Reset the device
        ResetDevice();
    }

    int InitializeWindow() {
        engine::logf("Engine Init");

        wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"Gaming", NULL };
        ::RegisterClassExW(&wc);
        hwnd = ::CreateWindowW(wc.lpszClassName, L"Obliviated Engine Beta", WS_OVERLAPPEDWINDOW, 100, 100, SCREENWIDTH, SCREENHEIGHT, NULL, NULL, wc.hInstance, NULL);

        // Initialize Direct3D
        if (!CreateDeviceD3D(hwnd))
        {
            CleanupDeviceD3D();
            ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
            return 1;
        }
        ::ShowWindow(hwnd, SW_SHOWDEFAULT);
        ::UpdateWindow(hwnd);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();

        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplDX9_Init(device);
    }

    void ShutDown() {
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        CleanupDeviceD3D();
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    }

    struct FullscreenVertex
    {
        float x, y, z, rhw;
        float tu, tv; // Texture coordinates

        FullscreenVertex(float _x, float _y, float _z, float _rhw, float _tu, float _tv)
            : x(_x), y(_y), z(_z), rhw(_rhw), tu(_tu), tv(_tv) {}
    };

    // Define the vertices for a fullscreen quad
    FullscreenVertex fullscreenQuad[] =
    {
        FullscreenVertex(-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
        FullscreenVertex(-1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
        FullscreenVertex(1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
        FullscreenVertex(1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
    };

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    D3DVIEWPORT9 viewport;
    int FullRender() {
        auto start = std::chrono::high_resolution_clock::now();

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        engine::invokeOnDrawUi();

        engine::invokeOnUpdate();


        ImGui::EndFrame();



        // update camera and transform the vertexes
        mainCamera->Update();


        if (device->BeginScene() >= 0)
        {
            // sets the vertex buffer
            engine::newFrame();

            // sets render target to texture
            device->SetRenderTarget(0, pRenderTargetSurface);
            viewport.X = 0;
            viewport.Y = 0;
            viewport.Width = RENDERWIDTH;    // Set to the width of your render target
            viewport.Height = RENDERHEIGHT;  // Set to the height of your render target
            viewport.MinZ = 0.0f;
            viewport.MaxZ = 1.0f;
            device->SetViewport(&viewport);
            device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
            
            device->SetRenderState(D3DRS_ZENABLE, TRUE);
            device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
            // sets texture
            device->SetTexture(0, pRenderTargetTexture);
            
            // triangle will not draw here



            
            // all rendering


            device->SetRenderTarget(0, pBackBufferSurface);
            viewport.X = 0;
            viewport.Y = 0;
            viewport.Width = SCREENWIDTH;    // Set to the width of your render target
            viewport.Height = SCREENHEIGHT;  // Set to the height of your render target
            viewport.MinZ = 0.0f;
            viewport.MaxZ = 1.0f;
            device->SetViewport(&viewport);

            device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 0, 0), 1.0f, 0);
            device->SetRenderState(D3DRS_ZENABLE, TRUE);
            device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
            device->SetTexture(0, pRenderTargetTexture);
            device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

            // lock, copy, and unlock vertex buffer
            void* pVoid;
            v_buffer->Lock(0, 0, (void**)&pVoid, 0);
            memcpy(pVoid, fullscreenQuad, sizeof(fullscreenQuad));
            v_buffer->Unlock();

            // set stream source
            
            device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, fullscreenQuad, sizeof(FullscreenVertex));
            RenderGrid();
            invokeOnRender();

            drawTexturedSquare(0, 0, 300, 0xffffff, tex.lpdMat);

            engine::endFrame();
            // end the scene
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            device->EndScene();


        }
        HRESULT result = device->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();

        // calculate delta time
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;
        deltaTime = duration.count();
        return 1;
    }


    // Helper functions
    bool CreateDeviceD3D(HWND hWnd)
    {
        if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
            return false;

        // Create the D3DDevice
        ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
        g_d3dpp.Windowed = TRUE;
        g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
        g_d3dpp.EnableAutoDepthStencil = TRUE;
        g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        if (engine::getVsync())
            g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
        else
            g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
        if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &device) < 0)
            return false;

        return true;
    }

    
    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        switch (msg)
        {
        case WM_SIZE:
            if (device != NULL && wParam != SIZE_MINIMIZED)
            {
                g_d3dpp.BackBufferWidth = LOWORD(lParam);
                g_d3dpp.BackBufferHeight = HIWORD(lParam);
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);

                OnResize(width, height);
            }
            return 0;

        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
        }
        return ::DefWindowProc(hWnd, msg, wParam, lParam);
    }

    void CleanupDeviceD3D()
    {
        if (device) { device->Release(); device = NULL; }
        if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
        if (pRenderTargetSurface != nullptr)
            pRenderTargetSurface->Release();

        if (pRenderTargetTexture != nullptr)
            pRenderTargetTexture->Release();
    }

    void ResetDevice()
    {
        ImGui_ImplDX9_InvalidateDeviceObjects();
        HRESULT hr = device->Reset(&g_d3dpp);
        if (hr == D3DERR_INVALIDCALL)
            IM_ASSERT(0);
        ImGui_ImplDX9_CreateDeviceObjects();
    }

    void rendererInit() {
        logf("Renderer init");
        loadTexture("fred_texture", L"textures/cat.png", &tex);
        InitializeWindow();
        mainCamera = new Camera2D(SCREENWIDTH, SCREENHEIGHT, 0.0f, 1);

        device->CreateTexture(RENDERWIDTH, RENDERHEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pRenderTargetTexture, nullptr);
        pRenderTargetTexture->GetSurfaceLevel(0, &pRenderTargetSurface);

        

        // Assuming g_pd3dDevice is your Direct3D device
        device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBufferSurface);

        onDrawUI(&onDrawRendererUi);
    }


}
