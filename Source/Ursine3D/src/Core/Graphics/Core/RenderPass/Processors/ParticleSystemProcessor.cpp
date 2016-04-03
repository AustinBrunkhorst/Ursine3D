/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleSystemProcessor.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/
#include "UrsinePrecompiled.h"

#include "ParticleSystemProcessor.h"
#include "Renderable.h"
#include "GfxManager.h"

namespace ursine
{
    namespace graphics
    {
        ParticleSystemProcessor::ParticleSystemProcessor(bool renderVelocity)
            : m_velocity(renderVelocity)
        {
            m_renderableType = RENDERABLE_PS;
        }

        bool ParticleSystemProcessor::cullOperation(_DRAWHND handle, Camera &currentCamera)
        {
            UAssert(handle.Type_ == m_renderableType, "GfxEntityProcessor attempted to proces invalid type!");

            ParticleSystem &particleSystem = m_manager->renderableManager->GetRenderableByID<ParticleSystem>( handle.Index_ );

            // if inactive
            if (!particleSystem.GetActive())
                return true;

            if(m_velocity)
            {
                if(!particleSystem.GetVelocityOrient( ))
                    return true;
            }
            else if (particleSystem.GetVelocityOrient( ))
                return true;

            // if culled by camera mask
            if (currentCamera.CheckMask( particleSystem.GetRenderMask( ) ))
                return true;

            // return false as in DO NOT CULL ME
            return false;
        }

        void ParticleSystemProcessor::prepOperation(_DRAWHND handle, SMat4 &view, SMat4 &proj, Camera &currentCamera)
        {
            ParticleSystem &particleSystem = m_manager->renderableManager->GetRenderableByID<ParticleSystem>( handle.Index_ );

            // SET BLEND MODE ///////////////////////////////////////
            if (particleSystem.GetAdditive( ))
                m_manager->dxCore->SetBlendState( BLEND_STATE_ADDITIVE );
            else
                m_manager->dxCore->SetBlendState( BLEND_STATE_DEFAULT );

            // SET RENDER SPACE /////////////////////////////////////
            PointGeometryBuffer pgb;
            if (particleSystem.GetSystemSpace( ))
            {
                m_manager->bufferManager->MapTransformBuffer( SMat4::Identity( ) );
            }
            else
            {
                m_manager->bufferManager->MapTransformBuffer(particleSystem.GetTransform( ));
            }

            // SET COLOR ////////////////////////////////////////////
            // set color
            auto &color = particleSystem.GetColor( );
            pgb.cameraUp.x = color.r;
            pgb.cameraUp.y = color.g;
            pgb.cameraUp.z = color.b;
            pgb.cameraUp.w = color.a;
            m_manager->bufferManager->MapBuffer<BUFFER_POINT_GEOM>(
                &pgb, 
                SHADERTYPE_VERTEX
            );

            // SET TEXTURE //////////////////////////////////////////
            m_manager->textureManager->MapTextureByID( handle.Material_ );
        }

        void ParticleSystemProcessor::renderOperation(_DRAWHND handle, Camera &currentCamera)
        {
            ParticleSystem &particleSystem = m_manager->renderableManager->GetRenderableByID<ParticleSystem>( handle.Index_ );
            
            if (particleSystem.GetActiveParticleCount() > 0)
            {
                unsigned passCount;
                if (particleSystem.GetActiveParticleCount() > 1024)
                    passCount = (particleSystem.GetActiveParticleCount( ) / 1024) + 1;
                else
                    passCount = 1;

                if (currentCamera.GetRenderMode( ) == ViewportRenderMode::VIEWPORT_RENDER_FORWARD)
                    passCount = 1;

                unsigned totalParticlesToRender = particleSystem.GetActiveParticleCount( );

                for (unsigned x = 0; x < passCount; ++x)
                {
                    unsigned particlesInPass = 1024;

                    if (x == passCount - 1)
                        particlesInPass = particleSystem.GetActiveParticleCount( ) % 1024;

                    // bind particle data
                    ParticleBuffer pb;
                    memcpy(
                        &pb.data, 
                        &(particleSystem.GetGPUParticleData( )[ x * 1024 ]), 
                        sizeof(Particle_GPU) * particlesInPass
                    );

                    m_manager->bufferManager->MapBuffer<BUFFER_PARTICLEDATA>(
                        &pb, 
                        SHADERTYPE_VERTEX, 
                        13
                    );

                    if (particlesInPass != 0)
                        m_manager->shaderManager->Render( 6 * particlesInPass );

                    // update particles left to render
                    totalParticlesToRender -= particlesInPass;
                }
            }
        }
    }
}