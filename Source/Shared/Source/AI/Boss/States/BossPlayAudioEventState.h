/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossPlayAudioEventState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

#include <ResourceReference.h>

class BossPlayAudioEventState : public BossAIState
{
public:
    BossPlayAudioEventState(
        ursine::resources::ResourceReference &resource,
        ursine::TimeSpan delay = ursine::TimeSpan( 0 )
    );

    void Enter(BossAIStateMachine *machine) override;

private:
    ursine::resources::ResourceReference &m_resource;
    ursine::TimeSpan m_delay;
};
