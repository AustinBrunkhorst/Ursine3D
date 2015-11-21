/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Viewport.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Viewport.h"

#include <d3d11.h>

namespace ursine
{
    namespace graphics
    {
        void Viewport::Initialize()
        {
            m_color[ 0 ] = 0.f;
            m_color[ 1 ] = 0.f;
            m_color[ 2 ] = 0.f;
            m_color[ 3 ] = 1.f;

            m_width = 800;
            m_height = 600;

            m_positionX = 0;
            m_positionY = 0;
        }

        void Viewport::SetBackgroundColor(float r, float g, float b, float a)
        {
            m_color[ 0 ] = r;
            m_color[ 1 ] = g;
            m_color[ 2 ] = b;
            m_color[ 3 ] = a;
        }

        void Viewport::SetBackgroundColor(Color color)
        {
            m_color[ 0 ] = color.ByteR() / 255.f;
            m_color[ 1 ] = color.ByteG() / 255.f;
            m_color[ 2 ] = color.ByteB() / 255.f;
            m_color[ 3 ] = color.ByteA() / 255.f;
        }

        const float *Viewport::GetBackgroundColor()
        {
            return m_color;
        }

        void Viewport::GetDimensions(unsigned &width, unsigned &height)
        {
            width = m_width;
            height = m_height;
        }

        void Viewport::SetDimensions(unsigned width, unsigned height)
        {
            m_width = width;
            m_height = height;
        }

        void Viewport::GetPosition(unsigned &positionX, unsigned &positionY)
        {
            positionX = m_positionX;
            positionY = m_positionY;
        }

        void Viewport::SetPosition(unsigned positionX, unsigned positionY)
        {
            m_positionX = positionX;
            m_positionY = positionY;
        }

        D3D11_VIEWPORT Viewport::GetViewportData()
        {
            D3D11_VIEWPORT viewport;
            ZeroMemory(&viewport, sizeof(viewport));

            viewport.Width = static_cast<float>(m_width);
            viewport.Height = static_cast<float>(m_height);
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
            viewport.TopLeftX = static_cast<float>(m_positionX);
            viewport.TopLeftY = static_cast<float>(m_positionY);

            return viewport;
        }
    }
}
