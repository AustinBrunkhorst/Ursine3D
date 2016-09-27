/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PointLightProcessor.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/
#include "UrsinePrecompiled.h"

#include "PointLightProcessor.h"
#include "Renderable.h"
#include "GfxManager.h"

namespace ursine
{
    namespace graphics
    {
        PointLightProcessor::PointLightProcessor(void)
        {
            m_renderableType = RENDERABLE_LIGHT;
        }

        bool PointLightProcessor::cullOperation(_DRAWHND handle, Camera &currentCamera)
        {
            UAssert(handle.Type_ == m_renderableType, "GfxEntityProcessor attempted to proces invalid type!");

            Light &light = m_manager->renderableManager->GetRenderableByID<Light>( handle.Index_ );

            // if wrong type
            if (light.GetType( ) != Light::LIGHT_POINT)
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

        void PointLightProcessor::prepOperation(_DRAWHND handle, SMat4 &view, SMat4 &proj, Camera &currentCamera)
        {
            Light &pointLight = m_manager->renderableManager->GetRenderableByID<Light>( handle.Index_ );

            // get radius
            float radius = pointLight.GetRadius() * 2;

            //ps needs point light data buffer
            SMat4 transposedView = currentCamera.GetViewMatrix( ); //need to transpose view (dx11 gg)
            transposedView.Transpose( );
            SVec3 lightPosition = transposedView.TransformPoint( pointLight.GetPosition( ) );

            // LIGHT DATA ///////////////////////////////////////////
            PointLightBuffer pointB;
            pointB.lightPos = lightPosition.ToD3D( );
            pointB.lightRadius = pointLight.GetRadius( );
            pointB.intensity = pointLight.GetIntensity( );
            pointB.color.x = pointLight.GetColor( ).r;
            pointB.color.y = pointLight.GetColor( ).g;
            pointB.color.z = pointLight.GetColor( ).b;

            m_manager->bufferManager->MapBuffer<BUFFER_POINT_LIGHT>(
                &pointB, 
                SHADERTYPE_PIXEL
            );

            // LIGHT TRANSFORM //////////////////////////////////////
            SMat4 transform;
            transform *= SMat4( pointLight.GetPosition( ) );
            transform *= SMat4( radius, radius, radius );
            m_manager->bufferManager->MapTransformBuffer( transform );

            // DETERMINE CULLING MODE ///////////////////////////////
            SVec3 cameraPosition = currentCamera.GetPosition();
            SVec3 lightP = pointLight.GetPosition();
            SVec3 camLight = cameraPosition - lightP;
            float distance = camLight.LengthSquared();
            float radiusSqr = radius * radius;

            // use either frontface or backface culling, depending on if
            // camera is inside or outside sphere
            if ( radiusSqr > fabs( distance ) )
                m_manager->dxCore->SetRasterState( RASTER_STATE_SOLID_FRONTCULL );
            else
                m_manager->dxCore->SetRasterState( RASTER_STATE_SOLID_BACKCULL );
        }

        void PointLightProcessor::renderOperation(_DRAWHND handle, Camera &currentCamera)
        {
            Light light = m_manager->renderableManager->GetRenderableByID<Light>(handle.Index_);

            m_manager->shaderManager->Render(
                m_manager->modelManager->GetModel( INTERNAL_SPHERE )->GetMesh( 0 )->GetIndexCount( )
            );
        }
    }
}