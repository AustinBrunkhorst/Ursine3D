/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CommandQueueComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "CommandQueueComponent.h"

NATIVE_COMPONENT_DEFINITION(CommandQueue);

CommandQueue::CommandQueue(void) 
    : BaseComponent()
    , m_useRecorder(false)
    , m_recording(false)
{
}

void CommandQueue::OnInitialize(void)
{
}

void CommandQueue::AddCommand(const std::shared_ptr<Command> &command) 
{
    m_commandQueue.push(command);
}

void CommandQueue::SetRecording(const bool flag) 
{
    m_recording = flag;
}

void CommandQueue::UseRecorder(const bool flag) 
{
    m_useRecorder = flag;
}

void CommandQueue::Update(void)
{
    // call all stop executes
    for ( auto &command : m_stopQueue )
    {
        command->StopExecute(GetOwner());
    }

    m_stopQueue.clear();

    while ( m_commandQueue.size() > 0 )
    {
        auto command = m_commandQueue.top();
        m_commandQueue.pop();

        command->Execute(GetOwner());

        if ( m_recording )
        {
           /* var recordable = command as RecordableCommand;

            if ( recordable != null )
                MessageCenter.main.dispatch(new RecordedCommandMessage(recordable, gameObject));*/
        }
        else
            m_stopQueue.push_back(std::shared_ptr<Command>(command));
    }
}

void CommandQueue::RunAllCommands()
{
    
}