/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FireCommand.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "FireCommand.h"
#include <Components/CharacterFireControllerComponent.h>

FireCommand::FireCommand()
{
    m_weight = 0;
}

void FireCommand::Execute(ursine::ecs::Entity* receiver)
{
    auto *fireComponent = receiver->GetComponent<CharacterFireController>();

    if(fireComponent != nullptr)
    {
        fireComponent->SetFireState(true);
    }
}

void FireCommand::StopExecute(ursine::ecs::Entity* receiver)
{
    auto *fireComponent = receiver->GetComponent<CharacterFireController>();

    if ( fireComponent != nullptr )
    {
        fireComponent->SetFireState(false);
    }
}

void FireCommand::StartRecording(ursine::ecs::Entity* receiver)
{
    
}

void FireCommand::Record(ursine::ecs::Entity* receiver, const int time)
{
    
}

void FireCommand::RecordedExecutionPrep(ursine::ecs::Entity* receiver, const int time)
{
    
}