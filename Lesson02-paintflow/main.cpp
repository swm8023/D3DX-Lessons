#include <Windows.h>
#include <stdio.h>
#include <LibDX/dxwindow.h>

using namespace dx;

class AppWindown : public DxWindow {
public:
    AppWindown(HINSTANCE hInstance, std::string title, int width, int height, bool windowed) :
        DxWindow(hInstance, title, width, height, windowed) {
    }

    bool Setup() {
        // 建立水壶Mesh        
        D3DXCreateTeapot(Device(), &m_pMesh, 0);

        // 视图矩阵
        D3DXMATRIX View;
        D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
        D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);
        D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
        D3DXMatrixLookAtLH(&View, &position, &at, &up);
        Device()->SetTransform(D3DTS_VIEW, &View);

        // 投影矩阵
        D3DXMATRIX Projection;
        D3DXMatrixPerspectiveFovLH(&Projection, D3DX_PI * 0.3f, (float)m_iWidth / (float)m_iHeight, 1.0f, 1000.0f);
        Device()->SetTransform(D3DTS_PROJECTION, &Projection);

        // 视口变化，只在局部绘制
        D3DVIEWPORT9 vp = { 100, 100, 600, 400, 0, 1 };
        Device()->SetViewport(&vp);

        // 设置线框模式绘制
        Device()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        return true;
    }

    void Present() {
        if (Device() == NULL) {
            return;
        }
        Device()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0X00, 0X00, 0XFF), 1.0f, 0L);
        Device()->BeginScene();
        
        // 模型矩阵
        D3DXMATRIX World, trans, rotate;
        static float degree = 0.0f;
        degree += Deltatime() * 1.5;
        D3DXMatrixIdentity(&World);
        D3DXMatrixRotationY(&rotate, degree);
        D3DXMatrixTranslation(&trans, 2.0f, 0.0f, 0.0f);
        World = trans * rotate;
        Device()->SetTransform(D3DTS_WORLD, &World);
        // 绘制水壶
        m_pMesh->DrawSubset(0);

        Device()->EndScene();
        Device()->Present(0, 0, 0, 0);
    }
private:
    ID3DXMesh *m_pMesh = 0;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdline, int showCmd) {
    AppWindown *dw = new AppWindown(hInstance, "Lesson 02-paintflow", 800, 600, true);
    if (dw->Initialize()) {
        dw->Run();
        dw->Release();
    }
    return 0;
}