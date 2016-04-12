/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PointRendererPass.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "PointRendererPass.h"
#include "GfxManager.h"

namespace ursine
{
    namespace graphics
    {
        PointRendererPass::PointRendererPass(
            bool debugRender,
            std::string passName
        )
            : RenderPass(passName)
            , m_debugRender(debugRender)
        {
            m_fullscreenPass = true;
        }

        PointRendererPass::~PointRendererPass(void)
        {

        }

        void PointRendererPass::executePass(Camera &currentCamera, int index)
        {
            ID3D11Buffer *mesh, *indices;
            unsigned vertCount, indexCount;

            // construct meshes
            if ( !m_debugRender )
                m_manager->drawingManager->ConstructPointMesh(
                    vertCount, 
                    indexCount, 
                    &mesh, 
                    &indices
                );
            else
                m_manager->drawingManager->ConstructOverdrawPointMesh(
                    vertCount, 
                    indexCount, 
                    &mesh, 
                    &indices
                );
            
            // check if we have points to render
            if(vertCount <= 0)
                return;

            // grab camera matrices
            SMat4 view = currentCamera.GetViewMatrix( ), 
                  proj = currentCamera.GetProjMatrix( );

            // set up buffers
            PointGeometryBuffer pgb;
            pgb.cameraUp = SVec4( currentCamera.GetUp( ), 0 ).ToD3D( );
            pgb.cameraPosition = SVec4( currentCamera.GetPosition( ), 1 ).ToD3D( );

            // map geometry data
            m_manager->bufferManager->MapBuffer<BUFFER_POINT_GEOM>( 
                &pgb, 
                SHADERTYPE_GEOMETRY 
            );

            // map camera buffers
            m_manager->bufferManager->MapCameraBuffer(
                view,
                proj,
                SHADERTYPE_GEOMETRY
             );

            // map meshes
            m_manager->modelManager->BindMesh<PrimitiveVertex>(
                mesh, 
                indices
            );

            // render
            m_manager->shaderManager->Render( indexCount );
        }
    }
}