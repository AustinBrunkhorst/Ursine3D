/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CommandQueueComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/
#pragma once

#include <Component.h>
#include "GameEvents.h"



class CommandQueue
    : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    Meta(Enable)
    CommandQueue(void);

    ~CommandQueue(void);

    void OnInitialize(void) override;
   
    void Update(void);

    // Query for a command to see if it was activated last frame
    bool QueryCommand(const game::GameEvents commandEvent) const;

private:
    static const int COMMAND_OFFSET = game::COMMAND_START + 1;
    static const int COMMAND_COUNT = game::COMMAND_END - COMMAND_OFFSET;

    // any commands that went off last frame
    bool m_commandPool[ COMMAND_COUNT ];

    // any commands that have went off this frame
    bool m_nextCommandPool[ COMMAND_COUNT ];


    void UpdatePools(void);

#define ENUMERATE(eventName)    void CommandQueue::On##eventName(EVENT_HANDLER(game::eventName)); 

#include "CommandEvents.inl"

#undef ENUMERATE





}Meta(Enable, WhiteListMethods, DisplayName( "CommandQueue" ));

