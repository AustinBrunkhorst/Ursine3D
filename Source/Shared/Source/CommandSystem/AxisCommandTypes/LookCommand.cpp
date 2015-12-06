/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LookCommand.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "LookCommand.h"
#include <Components/CharacterControllerComponent.h>

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

    auto *controller = receiver->GetComponent<CharacterController>( );
    controller->SetLookDirection(m_axis);
}

void LookCommand::StopExecute(ursine::ecs::Entity* receiver)
{
    auto *controller = receiver->GetComponent<CharacterController>( );

    controller->SetLookDirection( ursine::Vec2(0, 0) );
}

void LookCommand::StartRecording(ursine::ecs::Entity* receiver)
{
    m_characterRot.clear( );
    m_camRot.clear( );

    m_playback = false;
}

void LookCommand::Record(ursine::ecs::Entity* receiver, const float time)
{
    auto *transform = receiver->GetTransform();

    m_characterRot.push_back(transform->GetLocalRotation( ));
}

void LookCommand::RecordedExecutionPrep(ursine::ecs::Entity* receiver, const float time)
{
    auto index = time - m_startTime;

    if ( index > m_characterRot.size() )
        index = m_characterRot.size() - 1;

    auto *transform = receiver->GetTransform();
    transform->SetLocalRotation(m_characterRot[ index ]);

    m_playback = true;
}
