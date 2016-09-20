#include "Precompiled.h"

#include "SetToStartOnAwakeFixComponent.h"

NATIVE_COMPONENT_DEFINITION( SetToStartOnAwakeFix );

SetToStartOnAwakeFix::SetToStartOnAwakeFix(void)
    : BaseComponent( ) { }

void SetToStartOnAwakeFix::OnSceneReady(ursine::Scene *scene)
{
    GetOwner( )->GetComponent<EntityAnimator>( )->JumpToStart( );
}
