#include "Precompiled.h"

#include "RecorderSystem.h"
#include "RecorderComponent.h"

#include <Application.h>

namespace ursine
{
	ENTITY_SYSTEM_DEFINITION(RecorderSystem);

	RecorderSystem::RecorderSystem(ecs::World *world)
		: EntitySystem( world )
		, m_roundTimer( 0.0f )
		, m_roundNumber( -1 )
		, m_running( false ) { }

	void RecorderSystem::SetRoundStart(std::vector<std::vector<TeamComponent*> > &teamCompnents)
	{
		m_roundTimer = 0.0f;

		// Increment the round number
		++m_roundNumber;

		while (m_recordings.size( ) < teamCompnents.size( ))
			m_recordings.emplace_back( );

		// reset the previous round's recording's live list
		int teamNum = 0;
		for (auto &teamRecording : m_recordings)
		{
			int recordingNum = 0;
			for (auto &recording : teamRecording)
			{
				// init the playback
				recording.first = teamCompnents[ teamNum ][ recordingNum ]->GetOwner( )->GetUniqueID( );
				recording.second.InitPlayback( );

				++recordingNum;
			}
			++teamNum;
		}

		// Add a new life to each team's recording
		teamNum = 0;
		for (auto &teamRecording : m_recordings)
		{
			teamRecording.push_back( std::make_pair(
				teamCompnents[ teamNum][ m_roundNumber ]->GetOwner( )->GetUniqueID( ), 
				Recording( ) 
			) );

			++teamNum;
		}

		m_running = true;
	}

	void RecorderSystem::RecordCommand(RecordableCommandPtr command, ecs::Entity* entity)
	{
		auto team = entity->GetComponent<TeamComponent>( );

		UAssert(team != nullptr, "Error: You're attempting to record something that does not have a team component.");

		auto teamNum = team->GetTeamNumber( );

		auto &recording = m_recordings[ teamNum ][ m_roundNumber ];

		recording.second.Record( m_roundTimer, command, entity );
	}

	void RecorderSystem::OnInitialize(void)
	{
		m_world->Listener( this )
			.On( ecs::WORLD_UPDATE, &RecorderSystem::onUpdate );
	}

	void RecorderSystem::OnRemove(void)
	{
		m_world->Listener( this )
			.Off( ecs::WORLD_UPDATE, &RecorderSystem::onUpdate );
	}

	void RecorderSystem::onUpdate(EVENT_HANDLER(ecs::World))
	{
		EVENT_ATTRS(ecs::World, ecs::WorldEventArgs);

		if (!m_running)
			return;

		auto dt = Application::Instance->GetDeltaTime( );

		m_roundTimer += dt;

		for (auto &teamRecording : m_recordings)
		{
			// For each life that is less than previous life
			for (int i = 0; i < m_roundNumber; ++i)
			{
				// Tell the recording to play itself
				auto &recordingPair = teamRecording[ i ];

				recordingPair.second.Play( m_roundTimer, m_world->GetEntityUnique( recordingPair.first ) );
			}
		}
	}
}
