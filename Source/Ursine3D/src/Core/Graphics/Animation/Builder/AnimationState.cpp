/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationState.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** - Hyung Jun Park - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "AnimationState.h"
#include "AnimationBuilder.h"
#include "Notification.h"

namespace ursine
{
	AnimationState::AnimationState()
		: m_name("")
		, m_timePos(0.0f)
		, m_transPos(0.0f)
		, m_animname("")
		, m_animation(nullptr)
	{}

	const std::string &AnimationState::GetName(void) const
	{
		return m_name;
	}

	void AnimationState::SetName(const std::string& name)
	{
		m_name = name;
	}
	
	float AnimationState::GetTimePosition(void) const
	{
		return m_timePos;
	}

	void AnimationState::SetTimePosition(const float position)
	{
		m_timePos = position;
	}

	void AnimationState::IncrementTimePosition(const float dt)
	{
		m_timePos += dt;
	}

	// change this to push_back or something that can handle multiple animations insdie
	const Animation* AnimationState::GetAnimation(void) const
	{
		return m_animation;
	}

	void AnimationState::SetAnimation(Animation* animation)
	{
		m_animation = animation;
	}

	const std::string &AnimationState::GetAnimationName(void) const
	{
		return m_animname;
	}
	
	void AnimationState::SetAnimationName(const std::string& name)
	{
		if ("" == name)
			return;
		m_animname = name;
		
		Animation* targetAnimation = AnimationBuilder::GetAnimationByName(m_animname);
		if (!targetAnimation)
		{
			NotificationConfig error;

			error.type = NOTIFY_ERROR;
			error.header = "Animation Doesn't exist";
			error.message = "To add animation into the state, animation should exist in the Animation List";

			EditorPostNotification(error);
			return;
		}

		m_animation = targetAnimation;
	}

	// make this can handle multiple names of animation name
	const Animation* AnimationState::GetAnimationByName(void) const
	{
		return m_animation;
	}

	void AnimationState::SetAnimationByName(const std::string& name)
	{
		Animation* targetAnimation = AnimationBuilder::GetAnimationByName(name);
		if (!targetAnimation)
		{
			m_animation = nullptr;
			return;
		}
		m_animation = targetAnimation;
	}

	const float &AnimationState::GetTransPosition(void) const
	{
		return m_transPos;
	}

	void AnimationState::SetTransPosition(const float& tPos)
	{
		m_transPos = tPos;
	}

	//void AnimationState::GetFrameByTime(std::vector<AnimationKeyframe> &f1, std::vector<AnimationKeyframe> &f2, float time) const
	//{
	//	if (m_animation)
	//		return;
	//
	//	// determine the 2 current keyframes to use
	//	// we assume that all frames exist, and that they were baked across all total keyframes
	//	unsigned frameCount = m_animation->GetRigKeyFrameCount();
	//	for (unsigned x = 0; x < frameCount - 1; ++x)
	//	{
	//		// get the two current keyframes
	//		const std::vector<AnimationKeyframe> &frame1 = m_animation->GetKeyframes(x);
	//		const std::vector<AnimationKeyframe> &frame2 = m_animation->GetKeyframes(x + 1);
	//
	//		// check if the current keyframe set holds the time value between them
	//		if (frame1[0].length <= time && time < frame2[0].length)
	//		{
	//			f1 = frame1;
	//			f2 = frame2;
	//			break;
	//		}
	//	}
	//}
}
