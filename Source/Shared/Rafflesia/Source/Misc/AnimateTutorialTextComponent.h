/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimateTutorialTextComponent.h
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <SpriteTextComponent.h>
#include <ComponentProperties.h>

class AnimateTutorialText : public ursine::ecs::Component
{
	NATIVE_COMPONENT;

public:
	AnimateTutorialText(void);
	~AnimateTutorialText(void);

	ursine::Array<std::string> textLines;

	float characterWriteCooldown;
	float characterDeleteCooldown;
	float lineEndCooldown;

private:
	void OnInitialize(void) override;

	void onUpdate(EVENT_HANDLER(World));

	enum class TypeState
	{
		Writing,
		Deleting,
		LineEnded
	};

	float m_timer;
	int m_index;
	TypeState m_state;
	ursine::ecs::SpriteText *m_spriteText;
} Meta(
    Enable
) EditorMeta(
    RequiresComponents( typeof( ursine::ecs::SpriteText ) )
);
