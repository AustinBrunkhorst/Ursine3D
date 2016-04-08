/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FacePlayerBoneTowardsMeComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "FacePlayerBoneTowardsMeComponent.h"

#include "FaceBoneTowardsTargetComponent.h"

#include <World.h>

NATIVE_COMPONENT_DEFINITION(FacePlayerBoneTowardsMe);

using namespace ursine;
using namespace ecs;

FacePlayerBoneTowardsMe::FacePlayerBoneTowardsMe(void)
    : BaseComponent( ) { }

FacePlayerBoneTowardsMe::~FacePlayerBoneTowardsMe(void)
{
    auto world = GetOwner( )->GetWorld( );

    if (world)
        world->Listener( this )
            .Off( WORLD_UPDATE, &FacePlayerBoneTowardsMe::onUpdate );
}

void FacePlayerBoneTowardsMe::OnSceneReady(Scene *scene)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &FacePlayerBoneTowardsMe::onUpdate );
}

void FacePlayerBoneTowardsMe::onUpdate(EVENT_HANDLER(World))
{
    if (!m_targetFinder.size( ))
    {
        m_targetFinder = GetOwner( )->GetRoot( )->GetComponentsInChildren<FaceBoneTowardsTarget>( );

        if (!m_targetFinder.size( ))
            return;
    }

    auto position = GetOwner( )->GetTransform( )->GetWorldPosition( );

    for (auto bone : m_targetFinder)
        bone->SetTargetPosition( position );
}
