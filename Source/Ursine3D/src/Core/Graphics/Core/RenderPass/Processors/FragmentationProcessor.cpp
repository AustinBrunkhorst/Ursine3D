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

#include "FragmentationProcessor.h"
#include "Renderable.h"
#include "GfxManager.h"

namespace ursine
{
    namespace graphics
    {
        FragmentationProcessor::FragmentationProcessor(void)
        {
            m_renderableType = RENDERABLE_MODEL3D;
        }

        bool FragmentationProcessor::cullOperation(_DRAWHND handle, Camera &currentCamera)
        {
            UAssert(handle.Type_ == m_renderableType , "GfxEntityProcessor attempted to proces invalid type!");

            Model3D &model = m_manager->renderableManager->GetRenderableByID<Model3D>( handle.Index_ );

            // if inactive
            if ( !model.GetActive() )
                return true;

            // skip if it doesn't fragment
            if( !model.GetDoesFragment( ) )
            {
                return true;
            }

            // if culed by camera mask
            if (currentCamera.CheckMask(model.GetRenderMask()))
                return true;

            // return false as in DO NOT CULL ME
            return false;
        }

        void FragmentationProcessor::prepOperation(_DRAWHND handle, SMat4 &view, SMat4 &proj, Camera &currentCamera)
        {
            Model3D &model = m_manager->renderableManager->GetRenderableByID<Model3D>( handle.Index_ );

            /////////////////////////////////////////////////////////
            // map transform
            m_manager->bufferManager->MapTransformBuffer( model.GetWorldMatrix( ), SHADERTYPE_GEOMETRY );

            /////////////////////////////////////////////////////////
            // map fragmentation data
            FragmentationVSBuffer fvb;
            FragmentationGSBuffer fgb;
            FragmentationPSBuffer fpb;

            auto &fragData = model.GetFragmentData( );

            fvb.normalOffset            = fragData.normalOffset;

            fgb.verticalForce           = fragData.verticalForce;
            fgb.horizontalForce         = fragData.horizontalForce;
            fgb.outwardForce            = fragData.outwardForce;
            fgb.gravityForce            = fragData.gravityForce;
            fgb.time                    = fragData.time;
            fgb.randomForce             = fragData.randomForce;
            fgb.spinScalar              = fragData.spinScalar;
            fgb.seed                    = fragData.seed;
            fgb.globalTime              = fragData.globalTime;

            fpb.pulseSpeed              = fragData.pulseSpeed;
            fpb.fadeAmount              = fragData.fadeAmount;
            fpb.time                    = fragData.time;
            fpb.endTime                 = fragData.maxTime;
            fpb.transparencyThreshold   = fragData.transparencyThreshold;
            fpb.globalTime              = fragData.globalTime;
            fpb.color                   = fragData.color.ToD3D( );

            m_manager->bufferManager->MapBuffer<BUFFER_FRAG_VS>(
                &fvb,
                SHADERTYPE_VERTEX,
                11
            );
            m_manager->bufferManager->MapBuffer<BUFFER_FRAG_GS>(
                &fgb,
                SHADERTYPE_GEOMETRY,
                11
            );
            m_manager->bufferManager->MapBuffer<BUFFER_FRAG_PS>(
                &fpb,
                SHADERTYPE_PIXEL,
                11
            );


            /////////////////////////////////////////////////////////
            // map texture offset
            TextureUVOffset offset;
            offset.diffuseUV = DirectX::XMFLOAT2( fragData.xUV, fragData.yUV );

            m_manager->bufferManager->MapBuffer<BUFFER_TEX_OFFSET>(
                &offset,
                SHADERTYPE_VERTEX,
                13
            );

            m_manager->bufferManager->MapBuffer<BUFFER_TEX_OFFSET>(
                &offset,
                SHADERTYPE_PIXEL,
                13
            );

            /////////////////////////////////////////////////////////
            // map matrix palette
            MatrixPalBuffer data;

            int index = 0;
            for (auto &x : model.GetMatrixPalette())
            {
                data.matPal.matPal[ index++ ] = x.ToD3D();
            }
            
            m_manager->bufferManager->MapBuffer<BUFFER_MATRIX_PAL>(
                &data,
                SHADERTYPE_VERTEX
            );


            /////////////////////////////////////////////////////////
            // map texture
            m_manager->textureManager->MapTextureByID( (model.GetFragmentTextureHandle( ) & 0xFFFF) );
            m_manager->textureManager->MapTextureByID( static_cast<unsigned>(model.GetNormalTextureHandle( ) & 0xFFFF), 1 );
        }

        void FragmentationProcessor::renderOperation(_DRAWHND handle, Camera &currentCamera)
        {
            Model3D &model = m_manager->renderableManager->GetRenderableByID<Model3D>( handle.Index_ );

            renderFullModel( 
                handle, 
                model.GetDebug( ) 
            );

        }

        void FragmentationProcessor::renderFullModel(_DRAWHND handle, bool renderDebug)
        {
            auto &model = m_manager->renderableManager->GetRenderableByID<Model3D>( handle.Index_ );
            auto *modelResource = m_manager->modelManager->GetModel( handle.Model_ );
            auto count = modelResource->GetMeshCount();
            

            // main rendering
            for (unsigned x = 0; x < count; ++x)
            {
                auto *mesh = modelResource->GetMesh( x );

                // set model
                m_manager->modelManager->BindModel(
                    handle.Model_, 
                    x 
                );

                // render
                m_manager->shaderManager->Render( 
                    m_manager->modelManager->GetModel( handle.Model_ )->GetMesh( x )->GetIndexCount( )
                );
            }
        }
    }
}