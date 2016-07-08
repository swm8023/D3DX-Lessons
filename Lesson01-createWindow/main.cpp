#include <Windows.h>
#include <stdio.h>
#include <LibDX/dxwindow.h>

using namespace dx;

class AppWindown : public DxWindow {
public:
    AppWindown(HINSTANCE hInstance, std::string title, int width, int height, bool windowed) :
        DxWindow(hInstance, title, width, height, windowed) {
    }

    void Present() {
        if (Device() == NULL) {
            return;
        }

        Device()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0xFF), 1.0f, 0L);
        Device()->BeginScene();
        Device()->EndScene();
        Device()->Present(0, 0, 0, 0);
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdline, int showCmd) {
    AppWindown *dw = new AppWindown(hInstance, "Lesson 01-Window", 800, 600, true);
    if (dw->Initialize()) {
        dw->Run();
        dw->Release();
    }
    return 0;
}