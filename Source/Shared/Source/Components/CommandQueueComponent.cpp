/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CommandQueueComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "CommandQueueComponent.h"
#include "RecordableCommand.h"
#include "RecorderSystem.h"
#include <SystemManager.h>

NATIVE_COMPONENT_DEFINITION(CommandQueue);

CommandQueue::CommandQueue(void) 
    : BaseComponent()
    , m_useRecorder(false)
    , m_recording(false)
{
    memset(&m_commandPool, 0, sizeof(m_commandPool));
    memset(&m_nextCommandPool, 0, sizeof(m_nextCommandPool));
}

CommandQueue::~CommandQueue(void)
{
    GetOwner( )->Listener(this)
#define ENUMERATE(eventName)     .Off(game::eventName, &CommandQueue::On##eventName)
#include "CommandEvents.inl"
#undef ENUMERATE
        ;  // need delimitter to end connecting
}


void CommandQueue::OnInitialize(void)
{
    GetOwner( )->Listener(this)
#define ENUMERATE(eventName)     .On(game::eventName, &CommandQueue::On##eventName)
#include "CommandEvents.inl"
#undef ENUMERATE
        ;  // need delimitter to end connecting
}

void CommandQueue::AddCommand(const std::shared_ptr<Command> &command) 
{
    m_commandQueue.push(command);
}

void CommandQueue::SetRecording(const bool flag) 
{
    m_recording = flag;
}

bool CommandQueue::IsRecording(void) const
{
    return m_recording;
}

void CommandQueue::UseRecorder(const bool flag) 
{
    m_useRecorder = flag;
}

void CommandQueue::Update(void)
{
    UpdatePools( );

    // call all stop executes
    for ( auto &command : m_stopQueue )
    {
        command->StopExecute(GetOwner());
    }

    m_stopQueue.clear();

    auto recordingSystem = GetOwner( )->GetWorld( )->GetEntitySystem( ursine::RecorderSystem );
    auto entity = GetOwner( );

    while (m_commandQueue.size( ) > 0)
    {
        auto command = m_commandQueue.top( );
        m_commandQueue.pop( );

        command->Execute( entity );

        if (m_recording)
        {
            auto recordable = std::dynamic_pointer_cast<RecordableCommand>( command );

            if (recordable)
            {
                recordingSystem->RecordCommand( recordable, GetOwner( ) );
            }
        }
        else if (!m_useRecorder)
            m_stopQueue.push_back( command );
    }
}

void CommandQueue::UpdatePools(void)
{
    memcpy( &m_commandPool, &m_nextCommandPool, sizeof(m_nextCommandPool));
    memset( &m_nextCommandPool, 0, sizeof(m_nextCommandPool) );
}

bool CommandQueue::QueryCommand(const game::GameEvents commandEvent) const
{
    int index = commandEvent - COMMAND_OFFSET;

    UAssert(index >= 0 && index < game::COMMAND_END, "The Command being queried for does not exist.");

    return m_commandPool[ index ];
}


#define ENUMERATE(eventName)    \
    void CommandQueue::On##eventName(EVENT_HANDLER(game::eventName))      \
    {                                                                     \
        m_nextCommandPool[ game::eventName - COMMAND_OFFSET ] = true;    \
    }                                           

#include "CommandEvents.inl"

#undef ENUMERATE


