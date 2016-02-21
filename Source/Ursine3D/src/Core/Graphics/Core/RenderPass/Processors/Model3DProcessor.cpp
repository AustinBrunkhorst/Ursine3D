/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** Model3DProcessor.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/
#include "UrsinePrecompiled.h"

#include "Model3DProcessor.h"
#include "Renderable.h"
#include "GfxManager.h"

namespace ursine
{
    namespace graphics
    {
        Model3DProcessor::Model3DProcessor(void)
        {
            m_renderableType = RENDERABLE_MODEL3D;
        }

        bool Model3DProcessor::cullOperation(_DRAWHND handle, Camera &currentCamera)
        {
            UAssert(handle.Type_ == m_renderableType || handle.Type_ == RENDERABLE_OVERDRAW, "GfxEntityProcessor attempted to proces invalid type!");

            Model3D model = m_manager->renderableManager->GetRenderableByID<Model3D>(handle.Index_);

            // if inactive
            if ( !model.GetActive() )
                return true;

            // if culed by camera mask
            if ( currentCamera.CheckMask(model.GetRenderMask()) )
                return true;

            // return false as in DO NOT CULL ME
            return false;
        }

        void Model3DProcessor::prepOperation(_DRAWHND handle, SMat4 &view, SMat4 &proj, Camera &currentCamera)
        {
            Model3D model = m_manager->renderableManager->GetRenderableByID<Model3D>(handle.Index_);

            /////////////////////////////////////////////////////////
            // map color
            Color c = model.GetColor();
            PrimitiveColorBuffer pcb;
            pcb.color.x = c.r;
            pcb.color.y = c.g;
            pcb.color.z = c.b;
            pcb.color.w = c.a;
            m_manager->bufferManager->MapBuffer<BUFFER_PRIM_COLOR>(&pcb, SHADERTYPE_PIXEL);

            /////////////////////////////////////////////////////////
            // material buffer 
            MaterialDataBuffer mdb;

            // get material data
            model.GetMaterialData(mdb.emissive, mdb.specularPower, mdb.specularIntensity);

            // set unique ID for this model
            int overdrw = model.GetOverdraw() == true ? 1 : 0;

            unsigned type = handle.Type_;

            if (type == RENDERABLE_OVERDRAW)
                type = 3;

            type = type & 0x3;

            //             16                8
            mdb.id = (handle.Index_) | (type << 12) | (overdrw << 15) | (1 << 11);

            // map buffer
            m_manager->bufferManager->MapBuffer<BUFFER_MATERIAL_DATA>(&mdb, SHADERTYPE_PIXEL);

            /////////////////////////////////////////////////////////
            // map matrix palette
            MatrixPalBuffer data;

            int index = 0;
            for ( auto &x : model.GetMatrixPalette() )
            {
                data.matPal.matPal[ index++ ] = x.ToD3D();
            }
            m_manager->bufferManager->MapBuffer<BUFFER_MATRIX_PAL, MatrixPalBuffer>(&data, SHADERTYPE_VERTEX);

            /////////////////////////////////////////////////////////
            // map texture
            m_manager->textureManager->MapTextureByID(handle.Material_);
        }

        void Model3DProcessor::renderOperation(_DRAWHND handle, Camera &currentCamera)
        {
            Model3D model = m_manager->renderableManager->GetRenderableByID<Model3D>(handle.Index_);

            if ( model.GetMeshIndex() == -1 )
                renderFullModel( handle, model.GetDebug( ) );
            else
                renderSection( handle, model.GetDebug( ) );
        }

        void Model3DProcessor::renderFullModel(_DRAWHND handle, bool renderDebug)
        {
            auto &model = m_manager->renderableManager->GetRenderableByID<Model3D>( handle.Index_ );
            auto count = m_manager->modelManager->GetModelMeshCount( handle.Model_ );
            auto *modelResource = m_manager->modelManager->GetModel( handle.Model_ );

            // main rendering
            for ( unsigned x = 0; x < count; ++x )
            {
                auto *mesh = modelResource->GetMesh( x );

                // map transform
                m_manager->bufferManager->MapTransformBuffer( model.GetWorldMatrix( ) );

                // set model
                m_manager->modelManager->BindModel( 
                    handle.Model_, 
                    x 
                );

                // render
                m_manager->shaderManager->Render( 
                    m_manager->modelManager->GetModelIndexcountByID( handle.Model_, x )
                );
            }

            // rendering debug lines
            if(renderDebug)
            {
                m_manager->dxCore->SetRasterState(RASTER_STATE_LINE_RENDERING);

                PrimitiveColorBuffer pcb;
                pcb.color.x = 0.75f;
                pcb.color.y = 0.75f;
                pcb.color.z = 0.45f;
                m_manager->bufferManager->MapBuffer<BUFFER_PRIM_COLOR>(&pcb, SHADERTYPE_PIXEL);

                MaterialDataBuffer mdb;
                mdb.emissive = 4;
                mdb.specularPower = 0;
                mdb.specularIntensity = 0;
                m_manager->bufferManager->MapBuffer<BUFFER_MATERIAL_DATA>(&mdb, SHADERTYPE_PIXEL);
                m_manager->textureManager->MapTextureByName("Blank");

                for ( unsigned x = 0; x < count; ++x )
                {
                    auto *mesh = modelResource->GetMesh(x);

                    // map transform
                    m_manager->bufferManager->MapTransformBuffer(model.GetWorldMatrix());

                    // set model
                    m_manager->modelManager->BindModel(
                        handle.Model_,
                        x
                    );

                    // render
                    m_manager->shaderManager->Render(
                        m_manager->modelManager->GetModelIndexcountByID(handle.Model_, x)
                    );
                }
            }
        }

        void Model3DProcessor::renderSection(_DRAWHND handle, bool renderDebug)
        {
            auto &model = m_manager->renderableManager->GetRenderableByID<Model3D>( handle.Index_ );
            auto count = m_manager->modelManager->GetModelMeshCount( handle.Model_ );
            auto *modelResource = m_manager->modelManager->GetModel( handle.Model_ );
            auto meshIndex = model.GetMeshIndex( );

            // map transform
            m_manager->bufferManager->MapTransformBuffer( model.GetWorldMatrix() );

            // set model
            m_manager->modelManager->BindModel( handle.Model_, meshIndex );
            m_manager->shaderManager->Render( m_manager->modelManager->GetModelIndexcountByID(handle.Model_, meshIndex ));

            // debug rendering
            if( renderDebug )
            {
                m_manager->dxCore->SetDepthState(DEPTH_STATE_DEPTH_NOSTENCIL);
                m_manager->dxCore->SetRasterState(RASTER_STATE_LINE_RENDERING);

                PrimitiveColorBuffer pcb;
                pcb.color.x = 0.75f;
                pcb.color.y = 0.75f;
                pcb.color.z = 0.45f;
                m_manager->bufferManager->MapBuffer<BUFFER_PRIM_COLOR>(&pcb, SHADERTYPE_PIXEL);

                MaterialDataBuffer mdb;
                mdb.emissive = 4;
                mdb.specularPower = 0;
                mdb.specularIntensity = 0;
                m_manager->bufferManager->MapBuffer<BUFFER_MATERIAL_DATA>(&mdb, SHADERTYPE_PIXEL);
                m_manager->textureManager->MapTextureByName("Blank");

                // set model
                 m_manager->modelManager->BindModel(handle.Model_, meshIndex);
                 m_manager->shaderManager->Render(m_manager->modelManager->GetModelIndexcountByID(handle.Model_, meshIndex));

                 m_manager->dxCore->SetRasterState(RASTER_STATE_SOLID_BACKCULL);
                 m_manager->dxCore->SetDepthState(DEPTH_STATE_DEPTH_NOSTENCIL);
            }
        }
    }
}