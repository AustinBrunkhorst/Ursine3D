/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorLightSystem.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EditorDebugDrawSystems.h"

#include <DebugSystem.h>

#include <SelectedComponent.h>
#include <CameraComponent.h>
#include <LightComponent.h>

ENTITY_SYSTEM_DEFINITION( LightDebugDrawSystem );
ENTITY_SYSTEM_DEFINITION( CameraDebugDrawSystem );

using namespace ursine;
using namespace ecs;

LightDebugDrawSystem::LightDebugDrawSystem(World* world)
    : FilterSystem( world, Filter( ).All<Selected, Light>( ) )
{
    SetUpdateType( WORLD_EDITOR_UPDATE );
}

void LightDebugDrawSystem::Process(Entity* entity)
{
    auto light = entity->GetComponent<Light>( );

    auto drawer = m_world->GetEntitySystem( DebugSystem );

    if (!drawer)
        return;

    auto lightType = light->GetLightType( );

    switch (lightType)
    {
		case LightType::Directional:
		{

			static const float circleRadius = 3.0f;
			static const float rayLength = 8.0f;
			static const int resolution = 8;

			auto lightTrans = light->GetOwner( )->GetTransform( );
			auto lightPos = light->GetPosition( );
			auto lightRight = lightTrans->GetRight( );
			auto lightUp = lightTrans->GetUp( );
			auto lightForward = lightTrans->GetForward( );

			drawer->DrawCircle( light->GetPosition( ), lightForward, circleRadius, Color::Yellow, 0.0f );

			float theta = 0.0f;
			float step = 360.0f / resolution;

			for (int i = 0; i < resolution; ++i)
			{
				SQuat q1( theta, lightForward );

				theta += step;

				auto p1 = lightPos + q1.Rotate( lightRight ) * circleRadius;
				auto p2 = p1 + lightForward * rayLength;

				drawer->DrawLine( p1, p2, Color::Gold, 0.0f );
			}

			break;
		}
		case LightType::Spot:
		{
			// Draw the ellipse that's formed by the angles
			// NEED: distance of projection (intensity?)
			// get length
            auto *transform = entity->GetTransform( );
            float height = transform->GetWorldScale( ).Z( );

            // from the size, we can determine the diameter of the circle at the bottom  
            float radius = height * sinf( light->GetSpotlightAngles( ).Y( ) );

            // get the light direction
            auto lightDir = light->GetDirection( );

            // get position
            auto lightPos = light->GetPosition( ); 

            // calculate bottom position
            auto bottomPos = lightPos + lightDir * height;

            // calculate orthogonal vectors
            SVec3 u, v;
            lightDir.GenerateOrthogonalVectors(u, v);

            // render the bottom circle
            drawer->DrawCircle(bottomPos, lightDir, radius, Color::Gold, 0.0f);

            // render the side lines
            drawer->DrawLine(lightPos, bottomPos + u, Color::Gold, 0.0f);
            drawer->DrawLine(lightPos, bottomPos - u, Color::Gold, 0.0f);
            drawer->DrawLine(lightPos, bottomPos + v, Color::Gold, 0.0f);
            drawer->DrawLine(lightPos, bottomPos - v, Color::Gold, 0.0f);

			break;
		}
		case LightType::Point:
		{
			drawer->DrawSphere( 
				light->GetPosition( ), light->GetRadius( ), 
				Color::Yellow, 0.0f 
			);



			break;
		}
    }

}

CameraDebugDrawSystem::CameraDebugDrawSystem(World* world)
    : FilterSystem( world, Filter( ).All<Selected, Camera>( ) )
{
    SetUpdateType( WORLD_EDITOR_UPDATE );
}

void CameraDebugDrawSystem::Process(Entity* entity)
{
    auto cam = entity->GetComponent<Camera>( );

	if (cam->IsEditorCamera( ))
		return;

	// debug draw this camera
	auto drawer = m_world->GetEntitySystem( DebugSystem );

	if (!drawer)
		return;

	auto look = cam->GetLook( );
	auto right = cam->GetRight( );
	auto up = cam->GetUp( );
	auto pos = entity->GetTransform( )->GetWorldPosition( );
	auto fov = cam->GetFOV( );

	auto viewSize = cam->GetViewportSize( );
	auto ratio = viewSize.Y( ) / viewSize.X( );

	float tanValue = tan( fov * 0.5f );
	auto nearW = 2.0f * (tanValue * cam->GetNearPlane( ));
	auto nearH = ratio * nearW;

	auto farW = 2.0f * (tanValue * cam->GetFarPlane( ));
	auto farH = ratio * farW;

	auto p0 = (pos + look * cam->GetNearPlane( )) - (right * nearW * 0.5f) - (up * nearH * 0.5f);
	auto p1 = p0 + right * nearW;
	auto p2 = p1 + up * nearH;
	auto p3 = p2 - right * nearW;

	auto p4 = (pos + look * cam->GetFarPlane( )) - (right * farW * 0.5f) - (up * farH * 0.5f);
	auto p5 = p4 + right * farW;
	auto p6 = p5 + up * farH;
	auto p7 = p6 - right * farW;

	drawer->DrawLine( p0, p1, Color::Yellow, 0.0f );
	drawer->DrawLine( p1, p2, Color::Yellow, 0.0f );
	drawer->DrawLine( p2, p3, Color::Yellow, 0.0f );
	drawer->DrawLine( p3, p0, Color::Yellow, 0.0f );

	drawer->DrawLine( p4, p5, Color::Yellow, 0.0f );
	drawer->DrawLine( p5, p6, Color::Yellow, 0.0f );
	drawer->DrawLine( p6, p7, Color::Yellow, 0.0f );
	drawer->DrawLine( p7, p4, Color::Yellow, 0.0f );

	drawer->DrawLine( p0, p4, Color::Yellow, 0.0f );
	drawer->DrawLine( p1, p5, Color::Yellow, 0.0f );
	drawer->DrawLine( p2, p6, Color::Yellow, 0.0f );
	drawer->DrawLine( p3, p7, Color::Yellow, 0.0f );
}
