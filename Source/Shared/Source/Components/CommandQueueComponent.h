/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CommandQueueComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/
#pragma once

#include <Component.h>
#include "Command.h"
#include <queue>
#include "GameEvents.h"



struct CommandComparison
{
    bool operator()(const std::shared_ptr<Command> &c1, const std::shared_ptr<Command> &c2) const
    {
        return c1->GetWeight() < c2->GetWeight();
    }
};

class CommandQueue
    : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    Meta(Enable)
    CommandQueue(void);

    ~CommandQueue(void);

    void OnInitialize(void) override;

    void AddCommand(const std::shared_ptr<Command> &comman);

    void SetRecording(const bool flag);
    bool IsRecording(void) const;

    void UseRecorder(const bool flag);
   
    void Update(void);

    // Query for a command to see if it was activated last frame
    bool QueryCommand(const game::GameEvents commandEvent) const;

private:
    std::priority_queue<std::shared_ptr<Command>, std::vector<std::shared_ptr<Command>>, CommandComparison> m_commandQueue;
    std::vector<std::shared_ptr<Command>> m_stopQueue;
    
    static const int COMMAND_OFFSET = game::COMMAND_START + 1;
    static const int COMMAND_COUNT = game::COMMAND_END - COMMAND_OFFSET;

    // any commands that went off last frame
    bool m_commandPool[ COMMAND_COUNT ];

    // any commands that have went off this frame
    bool m_nextCommandPool[ COMMAND_COUNT ];

    bool m_useRecorder;
    bool m_recording;

    void UpdatePools(void);

#define ENUMERATE(eventName)    void CommandQueue::On##eventName(EVENT_HANDLER(game::eventName)); 

#include "CommandEvents.inl"

#undef ENUMERATE





}Meta(Enable, WhiteListMethods, DisplayName( "CommandQueue" ));

