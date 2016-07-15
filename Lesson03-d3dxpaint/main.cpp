#include <Windows.h>
#include <stdio.h>
#include <LibDX/dxwindow.h>
#include <LibDX/log.h>

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

        // 设置线框模式绘制
        Device()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

        void *pVertexes, *pIndices;

        // 顶点数据和索引数据
        float vertexes[] = {
            // 正方形
            -1.0f, -1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            // 扇形
            -1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.3f, 0.8f, 0.0f,
            0.8f, 0.3f, 0.0f,
            1.0f, 0.0f, 0.0f
        };
        short indices[] = {
            0, 1, 2, 0, 2, 3, 0, 1, 2, 3, 4
        };

        // 准备缓存
        IDirect3DVertexBuffer9 *vertexBuffer;
        IDirect3DIndexBuffer9 *indexBuffer;
        Device()->CreateVertexBuffer(sizeof(vertexes), 0, D3DFVF_XYZ, D3DPOOL_MANAGED, &vertexBuffer, 0);
        Device()->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuffer, 0);

        vertexBuffer->Lock(0, sizeof(vertexes), &pVertexes, 0);
        memcpy((char*)pVertexes, vertexes, sizeof(vertexes));
        vertexBuffer->Unlock();

        indexBuffer->Lock(0, sizeof(indices), &pIndices, 0);
        memcpy((char*)pIndices, indices, sizeof(indices));
        indexBuffer->Unlock();

        // 绘制准备
        Device()->SetStreamSource(0, vertexBuffer, 0, 3 * sizeof(float));
        Device()->SetFVF(D3DFVF_XYZ);
        Device()->SetIndices(indexBuffer);

        return true;
    }

    void Present() {
        if (Device() == NULL) {
            return;
        }
        Device()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0X00, 0XFF, 0X00), 1.0f, 0L);
        Device()->BeginScene();

        D3DXMATRIX world;

        // 绘制正方形
        D3DXMatrixIdentity(&world);
        D3DXMatrixTranslation(&world, -2.0f, 0.0f, 0.0f);
        Device()->SetTransform(D3DTS_WORLD, &world);
        Device()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

        // 绘制扇形
        D3DXMatrixIdentity(&world);
        D3DXMatrixTranslation(&world, 2.0f, 0.0f, 0.0f);
        Device()->SetTransform(D3DTS_WORLD, &world);
        Device()->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 4, 0, 8, 6, 3);

        Device()->EndScene();
        Device()->Present(0, 0, 0, 0);
    }
private:
    ID3DXMesh *m_pMesh = 0;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdline, int showCmd) {
    AppWindown *dw = new AppWindown(hInstance, "Lesson 03-d3dpaint", 800, 600, true);
    if (dw->Initialize()) {
        dw->Run();
        dw->Release();
    }
    return 0;
}