/* ----------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CameraAnimatorNodeComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class CameraAnimatorNode : public ursine::ecs::Component
{
	NATIVE_COMPONENT;

	friend class CameraAnimator;

public:
	EditorField(
		float transitionToTime,
		GetTransitionToTime,
		SetTransitionToTime
	);

	CameraAnimatorNode(void);

	float GetTransitionToTime(void) const;
	void SetTransitionToTime(float seconds);

private:
	
	float m_transitionSeconds;

} Meta(Enable);
