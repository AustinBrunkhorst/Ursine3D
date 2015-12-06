/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MoveCommand.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "MoveCommand.h"
#include <RigidbodyComponent.h>
#include <CharacterControllerComponent.h>

RECORDABLE_COMMAND_DEFINITION( MoveCommand );

MoveCommand::MoveCommand() 
    : RecordableAxisCommand( ursine::Vec2(0, 0)) {}

MoveCommand::MoveCommand(const ursine::Vec2& axis) 
    : RecordableAxisCommand(axis)
{
}

void MoveCommand::Execute(ursine::ecs::Entity* receiver)
{
    auto *controller = receiver->GetComponent<CharacterController>();

    if ( !m_playback )
        controller->SetMoveDirection(m_axis);
    else
    {
        auto dir = m_position - controller->GetOwner( )->GetTransform( )->GetWorldPosition( );
    }
}

void MoveCommand::StopExecute(ursine::ecs::Entity* receiver)
{
    auto *controller = receiver->GetComponent<CharacterController>();

    controller->SetMoveDirection( ursine::Vec2(0, 0) );
    //receiver->GetComponent<ursine::ecs::Rigidbody>()->SetBodyType(ursine::ecs::BodyType::Static);
}
    

void MoveCommand::StartRecording(ursine::ecs::Entity* receiver)
{
    m_positionList.clear();
    m_playback = false;
}

void MoveCommand::Record(ursine::ecs::Entity* receiver, const float time)
{
    auto *transform = receiver->GetTransform();

    m_positionList.push_back(transform->GetWorldPosition());
}

void MoveCommand::RecordedExecutionPrep(ursine::ecs::Entity* receiver, const float time)
{
    auto index = time - m_startTime;

    if ( index >= m_positionList.size() )
        index = m_positionList.size() - 1;

    m_position = m_positionList[ index ];
    m_playback = true;

    //receiver->GetComponent<ursine::ecs::Rigidbody>()->SetBodyType(ursine::ecs::BodyType::Kinematic);
}