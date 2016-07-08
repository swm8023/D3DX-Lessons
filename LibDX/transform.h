#pragma once

#include <stdlib.h>
#include <d3d.h>
#include <d3dx9.h>

#include "node.h"

namespace dx {
    class Transform {
    public:
        Transform(Node *owner) : 
            m_pstOwner(owner) {
            px = py = pz = 0.0f;
            rx = ry = rz = 0.0f;
            sx = sy = sz = 1.0f;
        }
        ~Transform() {}

        D3DXMATRIX RecalLocalMM() {
            // ���¼�������ģ�;���
            D3DXMATRIX result, move, rotateX, rotateY, rotateZ, scale;
            D3DXMatrixIdentity(&move);
            D3DXMatrixIdentity(&rotateX);
            D3DXMatrixIdentity(&rotateY);
            D3DXMatrixIdentity(&rotateZ);
            D3DXMatrixIdentity(&scale);
            D3DXMatrixScaling(&scale, sx, sy, sz);
            if (rx != 0.0f) {
                D3DXMatrixRotationX(&rotateX, rx);
            }
            if (ry != 0.0f) {
                D3DXMatrixRotationY(&rotateY, ry);
            }
            if (rz != 0.0f) {
                D3DXMatrixRotationZ(&rotateZ, rz);
            }
            D3DXMatrixTranslation(&move, px, py, pz);
            return m_stLocalMM = scale * rotateX * rotateY * rotateZ * move;
        }

        D3DXMATRIX RecalWorldMM() {
            // ���¼���ģ�;�������и��ף���Ҫ�˸��ڵ�ı任����
            if (m_pstOwner->Parent()) {
                return m_stWorldMM = RecalLocalMM() * m_pstOwner->Parent()->Trans()->WorldMM();
            } else {
                return m_stWorldMM = RecalLocalMM();
            }
        }

        D3DXMATRIX WorldMM() { return m_stWorldMM; }
        D3DXMATRIX LocalMM() { return m_stLocalMM; }

        float px, py, pz;
        float rx, ry, rz;
        float sx, sy, sz;

    private:
        D3DXMATRIX m_stLocalMM;
        D3DXMATRIX m_stWorldMM;

        Node *m_pstOwner;
    };
}

