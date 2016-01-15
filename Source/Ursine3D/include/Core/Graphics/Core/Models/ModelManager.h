/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ModelManager.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      ModelManager.h
Module:         Graphics
Purpose:        Manager for handling all of the models
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include <map>
#include <string>

#include "Model.h"
#include "ModelInfo.h"

namespace ursine
{
    namespace graphics
    {
        class ModelManager
        {
        public:
            void Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath);
            void Uninitialize();

            void LoadModel(std::string name, std::string fileName);
			void LoadModel_Fbx(std::string name, std::string fileName);
			void LoadModel_Ursine(std::string name, std::string fileName); 
			void LoadLevel_Ursine(std::string name, std::string fileName);

            ID3D11Buffer *GetModelVert(std::string name, unsigned index = 0);
            unsigned GetModelVertcount(std::string name, unsigned index = 0);
			unsigned GetModelIndexcount(std::string name, unsigned index = 0);

            void BindModel(std::string name, unsigned index = 0);
            void BindModel(unsigned ID, unsigned index = 0);

            //manual binding
            template<typename T>
            void BindMesh(ID3D11Buffer *mesh, ID3D11Buffer *indices)
            {
                m_currentState = -1;

                //map mesh
                unsigned int strides = sizeof(T);
                unsigned int offset = 0;

                m_deviceContext->IASetVertexBuffers(0, 1, &mesh, &strides, &offset);
                m_deviceContext->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, 0);
            }

            unsigned GetModelIDByName(std::string name);

            ID3D11Buffer *GetModelVertByID(unsigned ID, unsigned index = 0);
            unsigned GetModelVertcountByID(unsigned ID, unsigned index = 0);

			unsigned GetModelIndexcountByID(unsigned ID, unsigned index = 0);

            unsigned GetModelMeshCount(unsigned ID);

            void Invalidate();

            ModelResource *GetModel(const unsigned ID);
            ModelResource *GetModel(const std::string &name);

        private:

            ID3D11Device *m_device;
            ID3D11DeviceContext *m_deviceContext;

            std::map<std::string, ModelResource *> m_modelArray;
            std::map<std::string, unsigned> m_s2uTable;
            std::map<unsigned, ModelResource *> m_u2mTable;

            unsigned m_modelCount;
            unsigned m_currentState;
        };
    }
}