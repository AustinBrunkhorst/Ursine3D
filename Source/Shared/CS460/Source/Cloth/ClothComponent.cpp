/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ClothComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ClothNode.h"

#include <DebugSystem.h>
#include <Application.h>

using namespace ursine;
using namespace ecs;

NATIVE_COMPONENT_DEFINITION( Cloth );

Cloth::Cloth(void)
    : BaseComponent( )
    , m_dimensions( 1, 1 )
    , m_segmentLength( 1.0f )
    , m_logic( m_dimensions, m_segmentLength )
    , m_debug( false ) { }

const Vec2 &Cloth::GetDimensions(void) const
{
    return m_dimensions;
}

void Cloth::SetDimensions(const Vec2 &dimensions)
{
    Vec2 newDimensions = Vec2::Max(Vec2(1, 1), dimensions);

    if (newDimensions != m_dimensions)
    {
        m_logic.UpdateDimensions( newDimensions, m_segmentLength );
    }

    m_dimensions = newDimensions;

    NOTIFY_COMPONENT_CHANGED( "dimensions", m_dimensions );
}

float Cloth::GetSegmentLength(void) const
{
    return m_segmentLength;
}

void Cloth::SetSegmentLength(float segmentLength)
{
    if (math::IsZero( segmentLength ))
    {
        segmentLength = 1.0f;
    }

    if (!math::IsEqual( segmentLength, m_segmentLength ))
    {
        m_logic.UpdateDimensions( m_dimensions, segmentLength );
    }

    m_segmentLength = segmentLength;

    NOTIFY_COMPONENT_CHANGED( "segmentLength", m_segmentLength );
}

#if defined(URSINE_WITH_EDITOR)

bool Cloth::GetDebugDraw(void) const
{
    return m_debug;
}

void Cloth::SetDebugDraw(bool active)
{
    m_debug = active;

    NOTIFY_COMPONENT_CHANGED("debugDraw", m_debug);
}

void Cloth::DebugDraw(void)
{
    m_logic.Update( Application::Instance->GetDeltaTime( ) );

    // TODO: Change the name of DebugSystem to be DebugDrawer, and change DebugDrawer to PhysicsDebugDrawer
    auto debugDrawer = GetOwner( )->GetWorld( )->GetEntitySystem<DebugSystem>( );

    for (uint x = 0, w = m_logic.GetWidth( ); x < w; ++x)
    {
        for (uint y = 0, h = m_logic.GetHeight( ); y < h; ++y)
        {
            auto transform = GetOwner( )->GetTransform( )->GetLocalToWorldMatrix( );

            auto node = m_logic.GetNode( x, y );
            auto nodePosition = transform.TransformPoint( node->position );

            if ((x == 0 && y == 0) || x + 1 == w && y == 0)
            {
                debugDrawer->DrawCube(nodePosition, 2.0f, Color::Red, 0.0f, true);
            }
            
            auto left = m_logic.GetNode( x - 1, y );

            if (left)
            {
                auto leftPosition = transform.TransformPoint( left->position );

                debugDrawer->DrawLine( leftPosition, nodePosition, Color::Gold, 0.0f, true );
            }

            auto up = m_logic.GetNode(x, y - 1);

            if (up)
            {
                auto upPosition = transform.TransformPoint( up->position );

                debugDrawer->DrawLine( upPosition, nodePosition, Color::Gold, 0.0f, true );
            }
        }
    }
}

#endif
