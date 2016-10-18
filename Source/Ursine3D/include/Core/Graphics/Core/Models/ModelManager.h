/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ModelManager.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <string>
#include <atomic>

#include "ModelResource.h"
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
            GfxHND CreateModel(const resources::ModelData::Handle &modelData);
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

            // invalidate current state
            void Invalidate();

            ModelResource *GetModel(GfxHND handle);
            ModelResource *GetModelIndex(uint index);
            ModelResource *GetModel(const std::string &name);

            bool IsLoading(void) const;

            void EndFrame(void);

        private:
            void waitForLoading(void) const;
            void loadModelToGPU(ModelResource *model);
            void unloadModelFromGPU(ModelResource *model);

            ID3D11Device *m_device;
            ID3D11DeviceContext *m_deviceContext;

            // model
            unsigned m_nextModelID;
            unsigned m_currentState;

            std::atomic<bool> m_loadingModel;

            std::unordered_map<uint, ModelResource *> m_modelCache;
            std::unordered_map<std::string, GfxHND> m_modelTable;

            std::stack<uint> m_unloadStack;
        };
    }
}
