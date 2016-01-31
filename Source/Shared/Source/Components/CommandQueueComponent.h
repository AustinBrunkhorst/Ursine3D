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
#include "Command.h"
#include <queue>

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

    void OnInitialize(void) override;

    void AddCommand(const std::shared_ptr<Command> &comman);

    void SetRecording(const bool flag);
    bool IsRecording(void) const;

    void UseRecorder(const bool flag);
   
    void Update(void);

private:
    std::priority_queue<std::shared_ptr<Command>, std::vector<std::shared_ptr<Command>>, CommandComparison> m_commandQueue;
    std::vector<std::shared_ptr<Command>> m_stopQueue;

    bool m_useRecorder;
    bool m_recording;

}Meta(Enable, WhiteListMethods, DisplayName( "CommandQueue" ));

