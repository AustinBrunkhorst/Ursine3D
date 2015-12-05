#pragma once

#include <vector>
#include "RecordableCommand.h"

typedef std::shared_ptr<RecordableCommand> RecordableCommandPtr;

class Recording
{
public:
	typedef unsigned int RecordingIndex;

	Recording(void);

	void InitPlayback(void);

	void Play(float timeMarker, ursine::ecs::Entity *receiver);

	void Record(float roundTime, RecordableCommandPtr command, ursine::ecs::Entity *receiver);

	void UpdateRecording(float roundTime, ursine::ecs::Entity *receiver);

private:

	// This is the persistant array of Recorded commands.
	// You could think of this as the Master copy.
	std::vector<RecordableCommandPtr> m_recording;

	// This is the "Live Commands" List. This keeps track of what commands
	// in the recording are currently being executed. This can be used during recording
	// or playback.
	std::vector<RecordingIndex> m_live;

	// This queue is iterated through and cleared each frame, calling "StopExecute"
	// on the commands that ended in the previous frame.
	std::vector<RecordingIndex> m_stopQueue;

	// The current playback index. This is set to zero at the start of playback,
	// and incremented to the command that has happened most recently in the recording
	RecordingIndex m_playbackIndex;
};
