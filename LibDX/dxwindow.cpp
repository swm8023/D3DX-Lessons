#include "dxwindow.h"
#include "log.h"

using namespace dx;

DxWindow::DxWindow(HINSTANCE instance, std::string title, int width, int height, bool windowed) 
    : m_hwnd(NULL),
    m_pD3D9(NULL),
    m_pDevice(NULL),
    m_hInstance(instance),
    m_strTitle(title),
    m_iWidth(width),
    m_iHeight(height),
    m_bWindowed(windowed)
    {
}

bool DxWindow::Initialize() {
    // 初始化窗口
    WNDCLASSEX wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = dx::WndProc;
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = m_hInstance;
    wc.hIcon         = LoadIcon(m_hInstance, IDI_APPLICATION);
    wc.hIconSm       = LoadIcon(m_hInstance, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = m_strTitle.c_str();

    // 注册窗口
    if (!RegisterClassEx(&wc)) {
        Message("RegisterClassEx Failed!");
        return false;
    }

    // 创建窗口
    m_hwnd = CreateWindowEx(NULL, m_strTitle.c_str(), m_strTitle.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0, m_iWidth, m_iHeight, 0, 0, m_hInstance, 0);
    if (!m_hwnd) {
        Message("CreateWindowEx Failed!");
        Release();
        return false;
    }

    // 显示并更新窗口
    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);

    // 初始化Dierte3D指针
    if (NULL == (m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION))) {
        Message("Direct3DCreate9 Failed!");
        Release();
        return false;
    }

    // 判断显卡是否支持硬件定点处理
    D3DCAPS9 caps;
    DWORD vp = 0;
    if (FAILED(m_pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps))) {
        Message("GetDeviceCaps Failed!");
        Release();
        return false;
    }
    if (caps.VertexProcessingCaps) {
        vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    } else {
        vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    // 填充DXD3的绘制参数
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.BackBufferWidth            = m_iWidth;
    d3dpp.BackBufferHeight           = m_iHeight;
    d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferCount            = 1;
    d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality         = 0;
    d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow              = m_hwnd;
    d3dpp.Windowed                   = m_bWindowed;
    d3dpp.EnableAutoDepthStencil     = true;
    d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
    d3dpp.Flags                      = 0;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;

    // 创建Dierect3D设备对象
    if (FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hwnd,
        vp, &d3dpp, &m_pDevice))) {
        Message("CreateDevice Failed!");
        Release();
        return false;
    }
    return true;
}

void DxWindow::Release() {
    if (m_pDevice) {
        m_pDevice->Release();
        m_pDevice = NULL;
    }
    if (m_pD3D9) {
        m_pD3D9->Release();
        m_pD3D9 = NULL;
    }
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
        m_hwnd = NULL;
    }
    UnregisterClass(m_strTitle.c_str(), m_hInstance);
}



void DxWindow::Run() {
    if (Setup() == false) {
        Cleanup();
        Message("Setup Failed!");
        return;
    }
    int prems = timeGetTime();
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            // 计算两次绘制之间的间隔
            int nowms = timeGetTime();
            deltaTime = (nowms - prems) / 1000.0f;
            prems = nowms;
            Present();
        }
    }
    Cleanup();
}

LRESULT CALLBACK dx::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        break;
    //case WM_LBUTTONDOWN:
    //	g_bLeftMousing = true;
    //	break;
    //case WM_LBUTTONUP:
    //	g_bLeftMousing = false;
    //	break;
    //case WM_RBUTTONDOWN:
    //	g_bRightMousing = true;
    //	break;
    //case WM_RBUTTONUP:
    //	g_bRightMousing = false;
    //	break;
    default:
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
