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
            void InitializeJdl(std::string fileText);
            void InitializeModel(std::string fileText);
            void Uninitialize();

            void LoadModel(std::string name, std::string fileName);
            void LoadModel_Fbx(std::string name, std::string fileName);
            void LoadModel_Jdl(std::string name, std::string fileName);

            void InitializeModel(ufmt_loader::ModelInfo *modelInfo, ModelResource* modelresource);

            // creating a model resource
            GfxHND CreateModel(ufmt_loader::ModelInfo *modelInfo);
            void DestroyModel(GfxHND &handle);

            // loading/unloading model
            void LoadModel(GfxHND handle);
            void UnloadModel(GfxHND handle);

            ufmt_loader::ModelInfo *GetModelInfo(GfxHND handle);

            ID3D11Buffer *GetModelVert(std::string name, unsigned index = 0);
            unsigned GetModelVertcount(std::string name, unsigned index = 0);
            unsigned GetModelIndexcount(std::string name, unsigned index = 0);

            void BindModel(std::string name, unsigned index = 0, bool indexOnly = false);
            void BindModel(unsigned ID, unsigned index = 0, bool indexOnly = false);

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


            // Animation
            // creating a anime 
            void InitializeAnimation(ufmt_loader::AnimInfo *animeInfo);

            GfxHND CreateAnimation(ufmt_loader::AnimInfo *animeInfo);
            void DestroyAnimation(GfxHND &handle);

            ufmt_loader::AnimInfo *GeAnimeInfo(GfxHND handle);

            unsigned GetAnimeIDByName(std::string name);

        private:
            void loadModelToGPU(ModelResource *model);
            void unloadModelFromGPU(ModelResource *model);

            ID3D11Device *m_device;
            ID3D11DeviceContext *m_deviceContext;

            // model
            unsigned m_modelCount;
            unsigned m_currentState;

            std::map< std::string, unsigned > m_s2uTable;
            std::map <unsigned, ModelResource * > m_u2mTable;
            std::map< std::string, ModelResource * > m_modelArray;
            std::vector< std::string > m_jdllist;

            std::vector< ModelResource * > m_modelCache;
            std::vector< ufmt_loader::ModelInfo * > m_modelInfoCache;

            // anime
            unsigned m_animeCount;

            std::map< std::string, unsigned > a_s2uTable;
            std::map< unsigned, ufmt_loader::AnimInfo * > a_u2aTable;
            std::map< std::string, ufmt_loader::AnimInfo * > m_animeArray;

            std::vector< ufmt_loader::AnimInfo * > m_animeInfoCache;
        };
    }
}