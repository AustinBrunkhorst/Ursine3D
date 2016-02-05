#include "Precompiled.h"

#include "StartRoomDirectorSystem.h"

ENTITY_SYSTEM_DEFINITION( StartRoomDirectorSystem );

StartRoomDirectorSystem::StartRoomDirectorSystem(ursine::ecs::World *world)
    : EntitySystem( world )
{

}

void StartRoomDirectorSystem::OnAfterLoad(void)
{
    printf( "ON LOAD LOL\n" );
}

void StartRoomDirectorSystem::OnRemove(void)
{
    
}