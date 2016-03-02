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

#pragma once

#include <map>
#include <string>

#include "ModelResource.h"
#include "ModelInfo.h"
#include "AnimationInfo.h"
#include "GfxDefines.h"

namespace ursine
{
    namespace graphics
    {
        class ModelManager
        {
        public:
            void Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath);
            void Uninitialize();

            // Some version of this needs to exist
            void BindModel(unsigned ID, unsigned index = 0, bool indexOnly = false);

            // KEEP
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

            // KEEP
            unsigned GetModelIndexcountByID(unsigned ID, unsigned index = 0);

            // KEEP
            unsigned GetModelMeshCount(unsigned ID);

            // KEEP
            void Invalidate();

            // KEPT
            ModelResource *GetModel(const unsigned ID);


            ///////////////////////////////////////////////////////////////////////////////////////
            // KEPT
            // this takes all info from modelInfo and loads it into a modelresource
            void InitializeModel(ufmt_loader::ModelInfo *modelInfo, ModelResource& modelresource);

            // creating a model resource
            GfxHND CreateModel(ufmt_loader::ModelInfo *modelInfo);
            void DestroyModel(GfxHND &handle);

            // loading/unloading model
            void LoadModel(GfxHND handle);
            void UnloadModel(GfxHND handle);
            ufmt_loader::ModelInfo *GetModelInfo(GfxHND handle);

            // Animation
            // creating a anime 
            GfxHND CreateAnimation(ufmt_loader::AnimInfo *animeInfo);
            void DestroyAnimation(GfxHND &handle);
            ufmt_loader::AnimInfo *GeAnimeInfo(GfxHND handle);

            /////////////////////////////////////////////////////////////////////////////////////////
            // DELETED DATA
            std::map< std::string, unsigned > m_s2uTable;
            std::map <unsigned, ModelResource * > m_u2mTable;
            std::map< std::string, ModelResource * > m_modelArray;
            std::vector< std::string > m_jdllist;
            std::map< std::string, unsigned > a_s2uTable;
            std::map< unsigned, ufmt_loader::AnimInfo * > a_u2aTable;
            std::map< std::string, ufmt_loader::AnimInfo * > m_animeArray;

            // KEPT DATA //////////////////////////////////////////////////////

        private:
            void loadModelToGPU(ModelResource *model);
            void unloadModelFromGPU(ModelResource *model);

            ID3D11Device *m_device;
            ID3D11DeviceContext *m_deviceContext;

            // model
            unsigned m_modelCount;
            unsigned m_currentState;

            std::vector< ModelResource * > m_modelCache;
            std::vector< ufmt_loader::ModelInfo * > m_modelInfoCache;

            // anime
            unsigned m_animeCount;
            std::vector< ufmt_loader::AnimInfo * > m_animeInfoCache;
        };
    }
}