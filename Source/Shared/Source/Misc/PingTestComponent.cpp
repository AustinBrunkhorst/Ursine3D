/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PingTest.cpp
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PingTestComponent.h"

namespace 
{
std::string Play = "Play";
std::string Heart = "HeartBeat";
std::string Blip = "Switches";
std::string StateGroup = "States";
std::string SwitchGroup = "blips";
std::string Low = "low";
std::string High = "high";
std::string Pingas = "pingas";
std::string None = "None";
std::string Small = "small";
std::string Large = "large";
std::string RTPC = "heartbeat_param";
}

NATIVE_COMPONENT_DEFINITION(Ping);

Ping::Ping(void)
	: BaseComponent( )
	, m_beat( 100.0f )
	, m_switch( High )
	, m_state( None )
		{ }


Ping::~Ping(void) { }

void Ping::OnInitialize(void)
{
}

void Ping::PlayPingas(void)
{
	auto *emitter = GetOwner( )->GetComponent<ursine::ecs::AudioEmitter>( );

	auto event = std::make_shared<ursine::ecs::AudioGeneric>( );

	event->name = Play;

	emitter->PushEvent( event );
}

void Ping::PlayHeartbeat(void)
{

	auto *emitter = GetOwner( )->GetComponent<ursine::ecs::AudioEmitter>( );

	auto event = std::make_shared<ursine::ecs::AudioGeneric>( );

	event->name = Heart;

	emitter->PushEvent( event );
}

void Ping::SetHeartbeatVal(void)
{

	auto *emitter = GetOwner( )->GetComponent<ursine::ecs::AudioEmitter>( );

	auto rtpc = std::make_shared<ursine::ecs::AudioRTPC>( );

	rtpc->parameter = RTPC;
	rtpc->val = m_beat;

	emitter->PushEvent( rtpc );
}

void Ping::PlayBlip(void)
{
	auto *emitter = GetOwner( )->GetComponent<ursine::ecs::AudioEmitter>( );

	auto event = std::make_shared<ursine::ecs::AudioGeneric>( );

	event->name = Blip;

	emitter->PushEvent( event );
}

float Ping::GetSize(void) const
{
	return m_beat;
}

void Ping::SetSize(float size)
{
	m_beat = size;
}

std::string Ping::GetState(void) const
{
	return m_state;
}

void Ping::SetState(std::string state)
{
	m_state = state;
	auto *emitter = GetOwner( )->GetComponent<ursine::ecs::AudioEmitter> ();

	auto event = std::make_shared<ursine::ecs::AudioState>( );

	event->stateGroup = StateGroup;
	event->audioState = state;

	emitter->PushEvent( event );
			
}

std::string Ping::GetSwitch() const
{
	return m_switch;
}

void Ping::SetSwitch(std::string swich)
{
	m_switch = swich;
	auto *emitter = GetOwner( )->GetComponent<ursine::ecs::AudioEmitter>( );

	auto event = std::make_shared<ursine::ecs::AudioSwitch>( );

	event->switchGroup = SwitchGroup;
	event->switchState = swich;

	emitter->PushEvent( event );
}