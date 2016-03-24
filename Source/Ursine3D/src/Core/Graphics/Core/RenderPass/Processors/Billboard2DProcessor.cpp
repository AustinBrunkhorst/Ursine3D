/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** Billboard2DProcessor.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/
#include "UrsinePrecompiled.h"

#include "Billboard2DProcessor.h"
#include "Renderable.h"
#include "GfxManager.h"

namespace ursine
{
    namespace graphics
    {
        Billboard2DProcessor::Billboard2DProcessor(void)
        {
            m_renderableType = RENDERABLE_BILLBOARD2D;
        }

        bool Billboard2DProcessor::cullOperation(_DRAWHND handle, Camera &currentCamera)
        {
            UAssert(handle.Type_ == m_renderableType, "GfxEntityProcessor attempted to proces invalid type!");

            Billboard2D &billboard = m_manager->renderableManager->GetRenderableByID<Billboard2D>( handle.Index_ );

            // if inactive
            if (!billboard.GetActive( ))
                return true;

            // if culed by camera mask
            if (currentCamera.CheckMask( billboard.GetRenderMask( ) ))
                return true;

            // return false as in DO NOT CULL ME
            return false;
        }

        void Billboard2DProcessor::prepOperation(_DRAWHND handle, SMat4 &view, SMat4 &proj, Camera &currentCamera)
        {
            Billboard2D &billboard = m_manager->renderableManager->GetRenderableByID<Billboard2D>( handle.Index_ );

            // SPRITE DATA //////////////////////////////////////////
            BillboardSpriteBuffer bsb;
            billboard.GetDimensions( 
                bsb.width, 
                bsb.height 
            );
            m_manager->bufferManager->MapBuffer<BUFFER_BILLBOARDSPRITE>(
                &bsb, 
                SHADERTYPE_GEOMETRY
            );

            // COLOR ////////////////////////////////////////////////
            Color c = billboard.GetColor( );
            PrimitiveColorBuffer pcb;
            pcb.color.x = c.r;
            pcb.color.y = c.g;
            pcb.color.z = c.b;
            pcb.color.w = c.a;
            m_manager->bufferManager->MapBuffer<BUFFER_PRIM_COLOR>(
                &pcb, 
                SHADERTYPE_PIXEL
            );

            // TRANSFORM ////////////////////////////////////////////
            m_manager->bufferManager->MapTransformBuffer(
                SMat4( billboard.GetPosition( ) )
            );

            MaterialDataBuffer mdb;

            // set unique ID for this model /////////////////////////
            int overdrw = billboard.GetOverdraw( ) == true ? 1 : 0;
            mdb.emissive = 1.f;
            mdb.id = (handle.Index_) | (handle.Type_ << 12) | (overdrw << 15) | (1 << 11);
            m_manager->bufferManager->MapBuffer<BUFFER_MATERIAL_DATA>(
                &mdb, 
                SHADERTYPE_PIXEL
            );

            // map texture //////////////////////////////////////////
            m_manager->textureManager->MapTextureByID( handle.Material_ );

            // set overdraw value ///////////////////////////////////
            if (handle.Overdraw_)
                m_manager->dxCore->SetDepthState( DEPTH_STATE_PASSDEPTH_WRITESTENCIL );
            else
                m_manager->dxCore->SetDepthState( DEPTH_STATE_DEPTH_NOSTENCIL );
        }

        void Billboard2DProcessor::renderOperation(_DRAWHND handle, Camera &currentCamera)
        {
            m_manager->shaderManager->Render( 1 );
        }
    }
}