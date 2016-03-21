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
            ModelManager(void);

            void Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath);
            void Uninitialize(void);

            // create/destry model
            bool CheckModelExistence(const std::string &modelName);
            GfxHND CreateModel(const ufmt_loader::ModelInfo &modelInfo);
            void DestroyModel(GfxHND &handle);

            // load/unload model from gpu
            void LoadModel(GfxHND handle);
            void UnloadModel(GfxHND handle);

            // bind model to the GPU
            void BindModel(unsigned ID, unsigned index = 0, bool indexOnly = false);

            template<typename T>
            void BindMesh(ID3D11Buffer *mesh, ID3D11Buffer *indices)
            {
                m_currentState = -1;

                //map mesh
                unsigned int strides = sizeof(T);
                unsigned int offset = 0;

                m_deviceContext->IASetVertexBuffers( 0, 1, &mesh, &strides, &offset );
                m_deviceContext->IASetIndexBuffer( indices, DXGI_FORMAT_R32_UINT, 0 );
            }

            // get for index data
            unsigned GetModelIndexcountByID(unsigned ID, unsigned index = 0);
            unsigned GetModelMeshCount(unsigned ID);

            // invalidate current state
            void Invalidate();

            // animation stuff
            bool CheckAnimExistence(const std::string &animeName);
            GfxHND CreateAnimation(const ufmt_loader::AnimInfo &animeInfo);
            void DestroyAnimation(GfxHND &handle);

            // getting info
            ufmt_loader::ModelInfo *GetModelInfo(GfxHND handle);
            ufmt_loader::AnimInfo *GeAnimeInfo(GfxHND handle);
            ModelResource *GetModel(const unsigned ID);

            // getting model info by name
            ufmt_loader::ModelInfo *GetModelInfoByName(const std::string &name);

        private:
            void InitializeModel(const ufmt_loader::ModelInfo &modelInfo, ModelResource &modelresource);
            void loadModelToGPU(ModelResource &model);
            void unloadModelFromGPU(ModelResource &model);

            ID3D11Device *m_device;
            ID3D11DeviceContext *m_deviceContext;

            // model
            unsigned m_nextModelID;
            unsigned m_currentState;

            std::unordered_map<unsigned, ModelResource> m_modelCache;
            std::unordered_map<unsigned, ufmt_loader::ModelInfo> m_modelInfoCache;

            std::unordered_map<std::string, ufmt_loader::ModelInfo*> m_modelInfoTable;

            // animation
            unsigned m_nextAnimationID;

            std::unordered_map<unsigned, ufmt_loader::AnimInfo> m_animeInfoCache;
        };
    }
}
