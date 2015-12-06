/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Recording.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "Recording.h"
#include "Entity.h"
#include "CommandQueueComponent.h"
#include <Application.h>

Recording::Recording(void)
	: m_playbackIndex( 0 )
{
	
}

void Recording::InitPlayback(void)
{
	m_live.clear( );
	m_stopQueue.clear( );
	m_playbackIndex = 0;
}

void Recording::Play(ursine::uint64 timeMarker, ursine::ecs::Entity *receiver)
{
	if (receiver == nullptr)
		return;

	// go through the stop queue and call stop on all commands
	for (auto index : m_stopQueue)
	{
		m_recording[ index ]->StopExecute( receiver );
	}

	m_stopQueue.clear( );

	// check the recording for new commands to add to the live list (setting duration timer to zero)
	// this is based on the current time in the round based on their start times
	for (int i = m_playbackIndex; i < m_recording.size( ); ++i)
	{
		auto command = m_recording[ i ];

		// If this command has happened already
		if (command->GetStartTime( ) <= timeMarker)
		{
			// Add it to live list
			m_live.push_back( i );

			// increment the playback index
			++m_playbackIndex;
		}
		else
		{
			// Nothing beyond this point will be playing
			break;
		}
	}

	// go through the live list
	// if the live item's duration is up, remove it
	// else, call preexecute, and pass it to the command queue
	for (int i = 0, n = m_live.size( ); i < n; ++i)
	{
		auto command = m_recording[ m_live[ i ] ];
		auto endTime = command->GetStartTime( ) + command->GetDuration( );

		// call pre execute and pass it to the command queue
		command->RecordedExecutionPrep( receiver, timeMarker );

		auto commandQueue = receiver->GetComponent<CommandQueue>();

		UAssert( commandQueue != nullptr, "Error: The receiver must have a PlayerCommandQueue component attached to it." );
		
		commandQueue->AddCommand( command );

		// If this command is finished, remove it
		if (timeMarker >= endTime)
		{
			m_stopQueue.push_back( m_live[ i ] );
			m_live.erase( m_live.begin( ) + i );

			--i;
			--n;
		}
	}
}

void Recording::Record(ursine::uint64 roundTime, RecordableCommandPtr command, ursine::ecs::Entity *receiver)
{
	if (receiver == nullptr)
        return;

    // check to see if the command is in the live list
    auto newType = command->GetTypeID( );
                
    for (auto recordableCommand : m_live)
    {
		auto &currentCommand = m_recording[ recordableCommand ];
        auto liveType = currentCommand->GetTypeID( );

        // If it's already in the live list
        if (newType == liveType)
        {
            // Update the duration
			currentCommand->SetDuration( roundTime - currentCommand->GetStartTime( ) );
                        
            // Call the record function
			currentCommand->Record( receiver, roundTime );

            return;
        }
    }

    // We've made it to this point, meaning this command is
    // a new one and should be added to the recording and live list
    m_recording.push_back( command );
    m_live.push_back( m_recording.size( ) - 1 );

    // set the command's start time
    command->SetStartTime( roundTime );

    // reset the duration
    command->SetDuration( ursine::Application::Instance->GetDeltaTime( ) );

    // Call the start recording function and the record function
    command->StartRecording( receiver );
    command->Record( receiver, roundTime );
}

void Recording::UpdateRecording(ursine::uint64 roundTime, ursine::ecs::Entity *receiver)
{
	if (receiver == nullptr)
        return;

    // go through the stop queue and call stop on all commands
    for (auto recordableCommand : m_stopQueue)
    {
		auto command = m_recording[ recordableCommand ];
        command->StopExecute( receiver );
    }

    m_stopQueue.clear( );

    // The time threshold for detemining if a command is stale or not (in frames)
    ursine::uint64 threshold = 2;

    // iterate through all live actions
    for (int i = 0, n = m_live.size( ); i < n; ++i)
    {
        // check to see if they are stale
        auto &command = m_recording[ m_live[ i ] ];
        auto lastUpdateTime = command->GetStartTime( ) + command->GetDuration( );
        auto difference = roundTime - lastUpdateTime;

        if (difference > threshold)
        {
            // This command is stale, remove it
            m_stopQueue.push_back( m_live[ i ] );
            m_live.erase( m_live.begin( ) + i );
            --i;
            --n;
        }
    }
}
