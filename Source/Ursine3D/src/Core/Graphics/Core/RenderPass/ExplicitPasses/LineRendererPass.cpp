/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** LineRendererPass.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "LineRendererPass.h"
#include "GfxManager.h"

namespace ursine
{
    namespace graphics
    {
        LineRendererPass::LineRendererPass(
            bool debugRender, 
            std::string passName
        )
            : RenderPass( passName )
            , m_debugRender( debugRender )
        {
            m_explicitOverride = true;
        }

        LineRendererPass::~LineRendererPass(void)
        {
        }

        void LineRendererPass::executePass(Camera &currentCamera)
        {
            ID3D11Buffer *mesh, *indices;
            unsigned vertCount, indexCount;

            if(!m_debugRender)
                m_manager->drawingManager->ConstructLineMesh(vertCount, indexCount, &mesh, &indices);
            else
                m_manager->drawingManager->ConstructOverdrawLineMesh(vertCount, indexCount, &mesh, &indices);

            if(indexCount == 0)
                return;

            m_manager->modelManager->BindMesh<PrimitiveVertex>(mesh, indices);
            m_manager->shaderManager->Render(indexCount);
        }
    }
}