/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DirectionalLightProcessor.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/
#include "UrsinePrecompiled.h"

#include "DirectionalLightProcessor.h"
#include "Renderable.h"
#include "GfxManager.h"

namespace ursine
{
    namespace graphics
    {
        DirectionalLightProcessor::DirectionalLightProcessor(void)
        {
            m_renderableType = RENDERABLE_LIGHT;
        }

        bool DirectionalLightProcessor::cullOperation(_DRAWHND handle, Camera &currentCamera)
        {
            UAssert(handle.Type_ == m_renderableType, "GfxEntityProcessor attempted to proces invalid type!");

            Light &light = m_manager->renderableManager->GetRenderableByID<Light>( handle.Index_ );

            // if wrong type
            if (light.GetType( ) != Light::LIGHT_DIRECTIONAL)
                return true;

            // if inactive
            if ( !light.GetActive() )
                return true;

            // if culed by camera mask
            if ( currentCamera.CheckMask(light.GetRenderMask()) )
                return true;

            // return false as in DO NOT CULL ME
            return false;
        }

        void DirectionalLightProcessor::prepOperation(_DRAWHND handle, SMat4 &view, SMat4 &proj, Camera &currentCamera)
        {
            Light &directionalLight = m_manager->renderableManager->GetRenderableByID<Light>( handle.Index_ );

            // precalculate some stuff
            SMat4 transposedView = currentCamera.GetViewMatrix();
            transposedView.Transpose();
            SVec3 lightDirection = transposedView.TransformVector( directionalLight.GetDirection( ) );
            Color lightColor = directionalLight.GetColor( );

            // LIGHT DATA ///////////////////////////////////////////
            DirectionalLightBuffer lightB;
            lightB.lightDirection.x = lightDirection.X( );
            lightB.lightDirection.y = lightDirection.Y( );
            lightB.lightDirection.z = lightDirection.Z( );
            lightB.intensity = directionalLight.GetIntensity( );
            lightB.lightColor = DirectX::XMFLOAT3(
                lightColor.r * lightB.intensity, 
                lightColor.g * lightB.intensity, 
                lightColor.b * lightB.intensity
            );

            m_manager->bufferManager->MapBuffer<BUFFER_DIRECTIONAL_LIGHT>(
                &lightB, 
                SHADERTYPE_PIXEL
            );
        }

        void DirectionalLightProcessor::renderOperation(_DRAWHND handle, Camera &currentCamera)
        {
            Light &light = m_manager->renderableManager->GetRenderableByID<Light>( handle.Index_ );
            m_manager->shaderManager->Render(
                m_manager->modelManager->GetModelIndex(INTERNAL_QUAD)->GetMesh(0)->GetIndexCount()
            );
        }
    }
}