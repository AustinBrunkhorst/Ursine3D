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

LookCommand::LookCommand() 
    : RecordableAxisCommand(ursine::Vec2(0,0)) {}

LookCommand::LookCommand(const ursine::Vec2& axis) 
    : RecordableAxisCommand(axis) {}

void LookCommand::Execute(ursine::ecs::Entity* receiver)
{
    if ( m_playback )
        return;

    auto *controller = receiver->GetComponent<CharacterController>( );
    controller->lookDir = m_axis;
}

void LookCommand::StopExecute(ursine::ecs::Entity* receiver)
{
    auto *controller = receiver->GetComponent<CharacterController>( );

    controller->lookDir = ursine::Vec2(0, 0);
}

void LookCommand::StartRecording(ursine::ecs::Entity* receiver)
{
    m_characterRot.clear( );
    m_camRot.clear( );

    m_playback = false;
}

void LookCommand::Record(ursine::ecs::Entity* receiver, const int time)
{
    auto *transform = receiver->GetTransform();

    m_characterRot.push_back(transform->GetLocalRotation( ));
}

void LookCommand::RecordedExecutionPrep(ursine::ecs::Entity* receiver, const int time)
{
    auto index = time - m_startTime;

    if ( index > m_characterRot.size() )
        index = m_characterRot.size() - 1;

    auto *transform = receiver->GetTransform();
    transform->SetLocalRotation(m_characterRot[ index ]);

    m_playback = true;
}