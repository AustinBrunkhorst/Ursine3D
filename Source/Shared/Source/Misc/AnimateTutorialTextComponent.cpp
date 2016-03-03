/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimateTutorialTextComponent.cpp
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "AnimateTutorialTextComponent.h"

#include<Application.h>

NATIVE_COMPONENT_DEFINITION( AnimateTutorialText );

using namespace ursine;
using namespace ecs;

AnimateTutorialText::AnimateTutorialText(void)
	: BaseComponent( )
	, characterWriteCooldown( 1.0f )
	, characterDeleteCooldown( 1.0f )
	, lineEndCooldown( 1.0f )
	, m_timer( 0.0f )
	, m_index( 0 )
	, m_state( TypeState::Writing )
{
}

AnimateTutorialText::~AnimateTutorialText(void)
{
	GetOwner( )->GetWorld( )->Listener( this )
		.Off( WORLD_UPDATE, &AnimateTutorialText::onUpdate );
}

void AnimateTutorialText::OnInitialize(void)
{
	GetOwner( )->GetWorld( )->Listener( this )
		.On( WORLD_UPDATE, &AnimateTutorialText::onUpdate );

	m_spriteText = GetOwner( )->GetComponent<SpriteText>( );
	m_spriteText->SetText( "" );
}

void AnimateTutorialText::onUpdate(EVENT_HANDLER(World))
{
	if (m_index >= textLines.Size( ))
	{
		GetOwner( )->Delete( );
	}

	float dt = Application::Instance->GetDeltaTime( );
	auto &currentLine = textLines[ m_index ];
	
	m_timer += dt;

	switch (m_state)
	{
		case TypeState::Writing:
		{
			auto curText = m_spriteText->GetText( );

			if (curText.size( ) >= currentLine.size( ))
				m_state = TypeState::LineEnded;
			else if (m_timer >= characterWriteCooldown)
			{
				curText += currentLine[ curText.size( ) ];
				m_spriteText->SetText( curText );
				m_timer = 0.0f;
			}

			break;
		}
		case TypeState::Deleting:
		{
			auto curText = m_spriteText->GetText( );

			if (curText.size( ) == 0)
			{
				m_state = TypeState::Writing;
				++m_index;
			}
			else if (m_timer >= characterDeleteCooldown)
			{
				curText.pop_back( );
				m_spriteText->SetText( curText );
				m_timer = 0.0f;				
			}

			break;
		}
		case TypeState::LineEnded:
		{
			if (m_timer >= lineEndCooldown)
			{
				m_timer = 0.0f;
				m_state = TypeState::Deleting;
			}

			break;
		}
	}
}
