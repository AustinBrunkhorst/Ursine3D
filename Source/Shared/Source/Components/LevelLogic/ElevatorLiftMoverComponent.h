/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ElevatorLiftMoverComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class ElevatorLiftMover : public ursine::ecs::Component
{
	NATIVE_COMPONENT;

public:
	EditorField(
		ursine::SVec3 startPosition,
		GetStartPosition,
		SetStartPosition
	);

	EditorField(
		ursine::SVec3 endPosition,
		GetEndPosition,
		SetEndPosition
	);

	EditorField(
		float duration,
		GetDuration,
		SetDuration
	);

	EditorButton(
		startMoving, "Start Moving"
	);

	EditorButton(
		reset, "Reset"
	);

	ElevatorLiftMover(void);

	const ursine::SVec3 &GetStartPosition(void) const;
	void SetStartPosition(const ursine::SVec3 &startPosition);

	const ursine::SVec3 &GetEndPosition(void) const;
	void SetEndPosition(const ursine::SVec3 &endPosition);

	float GetDuration(void) const;
	void SetDuration(float duration);

	void StartMoving(void);

private:
	ursine::SVec3 m_startPos;
	ursine::SVec3 m_endPos;

	float m_duration;

	ursine::TweenID m_tween;

} Meta(Enable);
