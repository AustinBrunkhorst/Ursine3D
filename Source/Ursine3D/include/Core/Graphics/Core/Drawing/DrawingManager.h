/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      DrawingManager.h
Module:         Graphics
Purpose:        Class for handling drawing primitive lines and points
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include "D3D11Forward.h"
#include <vector>

#include "VertexDefinitions.h"

namespace ursine
{
    class DrawingManager
    {
    public:
        void Initialize(ID3D11Device *device, ID3D11DeviceContext *devCon);
        void Uninitialize();

        void EndScene();

        void SetDrawColor( float x, float y, float z, float a );
        void SetDrawColor( const Color &color );
        Color &GetColor( );

        void SetSize(float width);
        float GetSize();

        void DrawPoint(float x, float y, float z);

        void DrawLine(float x0, float y0, float z0, float x1, float y1, float z1);

        void ConstructPointMesh(unsigned &vertCount, unsigned &indexCount, ID3D11Buffer **mesh, ID3D11Buffer **indices);
        void ConstructLineMesh(unsigned &vertCount, unsigned &indexCount, ID3D11Buffer **mesh, ID3D11Buffer **indices);

        bool CheckRenderPoints( );
        bool CheckRenderLines( );
    private:
        ID3D11Device *m_device;
        ID3D11DeviceContext *m_deviceContext;
        float m_size;
        Color m_color;

        std::vector<PrimitiveVertex> m_pointList;
        std::vector<PrimitiveVertex> m_lineList;

        ID3D11Buffer *m_vertPointBuffer;
        ID3D11Buffer *m_vertLineBuffer;
        ID3D11Buffer *m_indexBuffer;

        unsigned m_pointCount;
        unsigned m_lineCount;
    };
}
