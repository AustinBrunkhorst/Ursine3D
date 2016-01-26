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
#include "RecordableCommand.h"
#include "RecorderSystem.h"
#include <SystemManager.h>

NATIVE_COMPONENT_DEFINITION(CommandQueue);

CommandQueue::CommandQueue(void) 
    : BaseComponent()
    , m_useRecorder(false)
    , m_recording(false)
{
}

void CommandQueue::OnInitialize(void)
{
    Component::OnInitialize( );
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
