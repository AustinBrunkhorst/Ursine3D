#include "Precompiled.h"

#include "Recording.h"
#include "Entity.h"
#include "CommandQueueComponent.h"

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

void Recording::Play(float timeMarker, ursine::ecs::Entity *receiver)
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

void Recording::Record(float roundTime, RecordableCommandPtr command, ursine::ecs::Entity *receiver)
{
	
}
