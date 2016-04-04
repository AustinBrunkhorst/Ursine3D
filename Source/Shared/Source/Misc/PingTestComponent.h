#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PingTest.h
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <AudioEmitterComponent.h>



class Ping : public ursine::ecs::Component
{
NATIVE_COMPONENT;

public:
	Ping(void);
	~Ping(void);

	void OnInitialize(void) override;

	EditorButton(
		PlayPingas,
		"Play Sound"
		);

	EditorField(
		std::string state,
		GetState,
		SetState
		);

	EditorButton(
		PlayHeartbeat,
		"Play Heartbeat"
		);

	EditorButton(
		SetHeartbeatVal,
		"Set Heartbeat Value"
		);

	EditorField(
		float speed,
		GetSize,
		SetSize
		);

	EditorButton(
		PlayBlip,
		"Play Sound"
		);

	EditorField(
		std::string switchType,
		GetSwitch,
		SetSwitch
		);

	float GetSize(void) const;
	void SetSize(float size);

	std::string GetState(void) const;
	void SetState(std::string state);

	std::string GetSwitch(void) const;
	void SetSwitch(std::string swich);

private:
	float m_beat;
	std::string m_state;
	std::string m_switch;


} Meta(Enable, DisplayName("Ping"));
