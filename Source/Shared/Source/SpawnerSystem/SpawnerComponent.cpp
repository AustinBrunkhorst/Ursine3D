/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnerComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - CJ Payne
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SpawnerComponent.h"
#include "SpawnPatternContainerComponent.h"

NATIVE_COMPONENT_DEFINITION( Spawner );

Spawner::Spawner(void)
    : BaseComponent( )
{
}

AIArchetype Spawner::GetEnemyType(void) const
{
    return m_enemyType;
}

#if defined(URSINE_WITH_EDITOR)

void Spawner::createPattern(void)
{
    auto owner = GetOwner( );
    auto newPattern = owner->GetWorld( )->CreateEntity( "NewPattern" );

    owner->GetTransform( )->AddChildAlreadyInLocal( newPattern->GetTransform( ) );

    newPattern->AddComponent<SpawnPatternContainer>( );

    newPattern->EnableHierarchyChange( false );
}

#endif
