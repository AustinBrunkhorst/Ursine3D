/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LookCommand.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "LookCommand.h"
#include "GameEvents.h"
#include <CharacterControllerComponent.h>
#include <Model3DComponent.h>

RECORDABLE_COMMAND_DEFINITION( LookCommand );

LookCommand::LookCommand() 
    : RecordableAxisCommand(ursine::Vec2(0,0))
	, m_playback( false ) { }

LookCommand::LookCommand(const ursine::Vec2& axis) 
    : RecordableAxisCommand(axis) 
	, m_playback( false ) {}


void LookCommand::Execute(ursine::ecs::Entity* receiver)
{
    if ( m_playback )
        return;

    game::MovementEventArgs lookChange( m_axis );
    receiver->Dispatch( game::LOOK_COMMAND, &lookChange );

}

void LookCommand::StopExecute(ursine::ecs::Entity* receiver)
{

}

void LookCommand::StartRecording(ursine::ecs::Entity* receiver)
{
    m_characterRot.clear( );
    m_camRot.clear( );

    m_playback = false;
}

void LookCommand::Record(ursine::ecs::Entity* receiver, const ursine::uint64 time)
{
    auto *transform = receiver->GetTransform();

    m_characterRot.push_back(
        transform->GetComponentInChildren<ursine::ecs::Model3D>( )
        ->GetOwner( )->GetTransform( )->GetLocalRotation( )
    );
}

void LookCommand::RecordedExecutionPrep(ursine::ecs::Entity* receiver, const ursine::uint64 time)
{
    auto index = time - m_startTime;

    if ( index >= m_characterRot.size( ) )
        index = m_characterRot.size( ) - 1;

    auto *transform = receiver->GetComponentInChildren<ursine::ecs::Model3D>( )->GetOwner( )->GetTransform( );
    transform->SetLocalRotation( m_characterRot[ index ] );

    auto *controller = receiver->GetComponent<CharacterController>();
    controller->SetLookDirection( ursine::Vec2::Zero( ) );

    m_playback = true;
}
