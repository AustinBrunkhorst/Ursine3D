/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CameraAnimatorComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "CameraAnimatorComponent.h"
#include "CameraAnimatorNodeComponent.h"

#include <CameraComponent.h>
#include <Curves.h>
#include <SystemManager.h>
#include <DebugSystem.h>

NATIVE_COMPONENT_DEFINITION( CameraAnimator );

using namespace ursine;
using namespace ecs;

CameraAnimator::CameraAnimator(void)
    : BaseComponent( )
    , m_camera( nullptr )
    , m_playing( false )
    , m_smoothPath( false )
    , m_index( 0 )
    , m_time( 0.0f )
{
}

CameraAnimator::~CameraAnimator(void)
{
}

bool CameraAnimator::GetSmoothPath(void) const
{
    return m_smoothPath;
}

void CameraAnimator::SetSmoothPath(bool smooth)
{
    m_smoothPath = smooth;

    Reset( );
}

void CameraAnimator::Play(void)
{
    m_playing = true;
    m_time = 0.0f;
    m_index = 0;

    getChildren( );
    m_camera = GetOwner( )->GetComponentInChildren<Camera>( );

    enableDeletionNodes( false );
}

void CameraAnimator::Reset(void)
{
    m_index = 0;
    m_playing = false;
    m_time = 0.0f;

    getChildren( );
    m_camera = GetOwner( )->GetComponentInChildren<Camera>( );

    if (m_nodes.size( ))
        updateAnimation( m_nodes[ 0 ] );

    enableDeletionNodes( true );

    m_nodes.clear( );
}

void CameraAnimator::updateAnimation(CameraAnimatorNode *node)
{
    if (!m_camera || !node)
        return;

    auto trans = m_camera->GetOwner( )->GetTransform( );
    auto nodeTrans = node->GetOwner( )->GetTransform( );

    trans->SetWorldPosition( nodeTrans->GetWorldPosition( ) );
    trans->SetWorldRotation( nodeTrans->GetWorldRotation( ) );
}

void CameraAnimator::updateAnimation(CameraAnimatorNode *node1, CameraAnimatorNode *node2, float t)
{
    if (!m_camera || !node1 || !node2)
        return;

    auto trans = m_camera->GetOwner( )->GetTransform( );
    auto node1Trans = node1->GetOwner( )->GetTransform( );
    auto node2Trans = node2->GetOwner( )->GetTransform( );

    trans->SetWorldPosition(
        (1.0f - t) * node1Trans->GetWorldPosition( ) +
        t * node2Trans->GetWorldPosition( )
    );

    trans->SetWorldRotation( 
        node1Trans->GetLocalRotation( )
        .Slerp( node2Trans->GetLocalRotation( ), t )
    );
}

void CameraAnimator::updateAnimation(CameraAnimatorNode *node1, CameraAnimatorNode *node2,
                                     CameraAnimatorNode *node3, CameraAnimatorNode *node4, float t)
{
    if (!m_camera || !node1 || !node2 || !node3 || !node4)
        return;

    auto trans = m_camera->GetOwner( )->GetTransform( );
    
    auto node1Trans = node1->GetOwner( )->GetTransform( );
    auto node2Trans = node2->GetOwner( )->GetTransform( );
    auto node3Trans = node3->GetOwner( )->GetTransform( );
    auto node4Trans = node4->GetOwner( )->GetTransform( );

    trans->SetWorldPosition(
        ursine::Curves::CatmullRomSpline(
            node1Trans->GetWorldPosition( ),
            node2Trans->GetWorldPosition( ),
            node3Trans->GetWorldPosition( ),
            node4Trans->GetWorldPosition( ),
            t
        )
    );

    trans->SetWorldRotation( 
        node2Trans->GetLocalRotation( )
        .Slerp( node3Trans->GetLocalRotation( ), t )
    );
}

void CameraAnimator::enableDeletionNodes(bool flag)
{
    for (auto *node : m_nodes)
    {
        node->GetOwner( )->EnableDeletion( flag );
    }
}

void CameraAnimator::getChildren(void)
{
    auto nodes = GetOwner( )->GetComponentsInChildren<CameraAnimatorNode>( );

    m_nodes.clear( );

    for (auto node : nodes)
    {
        int i = 0;
        bool found = false;

        for (; found == false && i < m_nodes.size( ); ++i)
            if (m_nodes[ i ]->m_order > node->m_order)
            {
                found = true;
                --i;
            }

        m_nodes.insert( m_nodes.begin( ) + i, node );
    }
}

#if defined(URSINE_WITH_EDITOR)

void CameraAnimator::play(void)
{
    Play( );
}

void CameraAnimator::reset(void)
{
    Reset( );
}

void CameraAnimator::drawPath(void)
{
    getChildren( );

    auto drawer = GetOwner( )->GetWorld( )->GetEntitySystem( ursine::ecs::DebugSystem );

    if (m_smoothPath)
    {
        for (int i = 0; i < m_nodes.size( ) - 1; ++i)
        {
            auto node0 = m_nodes[ i ]->GetOwner( )->GetTransform( )->GetWorldPosition( );
            auto node1 = m_nodes[ i + 1 ]->GetOwner( )->GetTransform( )->GetWorldPosition( );

            auto before = i == 0 ? node0 : m_nodes[ i - 1 ]->GetOwner( )->GetTransform( )->GetWorldPosition( );
            auto after = i + 2 >= m_nodes.size( ) ? node1 : m_nodes[ i + 2 ]->GetOwner( )->GetTransform( )->GetWorldPosition( );

            static const int precision = 20;
            float t = 0.0f;
            float step = 1.0f / precision;

            for (int j = 0; j < precision; ++j)
            {
                auto p0 = Curves::CatmullRomSpline( before, node0, node1, after, t );

                t += step;

                auto p1 = Curves::CatmullRomSpline( before, node0, node1, after, t );

                drawer->DrawLine( p0, p1, Color::Yellow, 3.0f );
            }
        }
    }
    else
    {
        for (int i = 0; i < m_nodes.size( ) - 1; ++i)
        {
            auto node0 = m_nodes[ i ]->GetOwner( )->GetTransform( )->GetWorldPosition( );
            auto node1 = m_nodes[ i + 1 ]->GetOwner( )->GetTransform( )->GetWorldPosition( );

            drawer->DrawLine( node0, node1, Color::Yellow, 3.0f );
        }
    }
}

#endif
