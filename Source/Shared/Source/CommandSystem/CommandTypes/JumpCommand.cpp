/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** JumpCommand.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "JumpCommand.h"
#include <AudioEmitterComponent.h>
#include "GameEvents.h"

namespace
{
    const std::string kJumpSound = "PLAYER_JUMP";
}

JumpCommand::JumpCommand(void)
{
    m_weight = 5;
}

void JumpCommand::Execute(ursine::ecs::Entity* receiver)
{
    URSINE_TODO("possibly move jump sound out of command");

    auto *emitter = receiver->GetComponent<ursine::ecs::AudioEmitter>();

    if (emitter)
        emitter->AddSoundToPlayQueue(kJumpSound);
    
    receiver->Dispatch(game::JUMP_COMMAND, ursine::EventArgs::Empty);
}

void JumpCommand::StopExecute(ursine::ecs::Entity* receiver)
{

}
