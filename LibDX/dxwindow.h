#pragma once
#include <d3dx9.h>
#include <d3d.h>
#include <string>

namespace dx {
    class DxWindow {
    public:
        DxWindow(HINSTANCE instance, std::string tiltle, 
            int width, int height, bool windowed);
        bool Initialize();
        void Release();
        void Run();

        // 获得D3DX设备指针
        inline LPDIRECT3DDEVICE9 Device() { return m_pDevice; }
        // 获得距离上次绘制的间隔
        inline float Deltatime() { return deltaTime; }

        // 需要重写的方法
        virtual bool Setup() { return true; }
        virtual void Cleanup() {};
        virtual void Present() = 0;

    protected:
        HWND m_hwnd;                // 窗口句柄
        HINSTANCE m_hInstance;      // 实例句柄
        LPDIRECT3D9 m_pD3D9;        // Direct3D接口对象
        LPDIRECT3DDEVICE9 m_pDevice;// Direct3D设备对象

        std::string m_strTitle;     // 窗口标题
        int m_iWidth;               // 窗口宽
        int m_iHeight;              // 窗口长
        int m_bWindowed;            // 是否窗口显示
        float deltaTime;            // 两次绘制的间隔
    };

    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
}

