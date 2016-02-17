/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

NATIVE_COMPONENT_DEFINITION( CameraAnimator );

using namespace ursine;
using namespace ecs;

CameraAnimator::CameraAnimator(void)
	: BaseComponent( )
	, m_camera( nullptr )
	, m_playing( false )
	, m_index( 0 )
	, m_time( 0.0f )
{

}

CameraAnimator::~CameraAnimator(void)
{

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

void CameraAnimator::updateAnimation(CameraAnimatorNode* node)
{
	if (!m_camera || !node)
		return;

	auto trans = m_camera->GetOwner( )->GetTransform( );
	auto nodeTrans = node->GetOwner( )->GetTransform( );

	trans->SetWorldPosition( nodeTrans->GetWorldPosition( ) );
	trans->SetWorldRotation( nodeTrans->GetWorldRotation( ) );
}

void CameraAnimator::updateAnimation(CameraAnimatorNode* node1, CameraAnimatorNode* node2, float t)
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

#endif
