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

        // ���D3DX�豸ָ��
        inline LPDIRECT3DDEVICE9 Device() { return m_pDevice; }
        // ��þ����ϴλ��Ƶļ��
        inline float Deltatime() { return deltaTime; }

        // ��Ҫ��д�ķ���
        virtual bool Setup() { return true; }
        virtual void Cleanup() {};
        virtual void Present() = 0;

    protected:
        HWND m_hwnd;                // ���ھ��
        HINSTANCE m_hInstance;      // ʵ�����
        LPDIRECT3D9 m_pD3D9;        // Direct3D�ӿڶ���
        LPDIRECT3DDEVICE9 m_pDevice;// Direct3D�豸����

        std::string m_strTitle;     // ���ڱ���
        int m_iWidth;               // ���ڿ�
        int m_iHeight;              // ���ڳ�
        int m_bWindowed;            // �Ƿ񴰿���ʾ
        float deltaTime;            // ���λ��Ƶļ��
    };

    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
}

