/* ---------------------------------------------------------------------------
** Team Bear King
** 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationState.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** - Hyung Jun Park - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once
#include "Animation.h"
//this is per-instance data that keeps track of the current state of our animation

namespace ursine
{
	class AnimationState
	{
	public:
		EditorField(
			std::string stateName,
			GetName,
			SetName
			);

		// how can I add multiple animations in animation state?
		// how can I use combo box for this?
		// Array<Animation> => no, in this case, we should edit
		EditorField(
			std::string animName,
			GetAnimationName,
			SetAnimationName
			);

		/** @brief constructor
		*
		*  @return Void.
		*/
		AnimationState(void);

		const std::string &GetName(void) const;
		void SetName(const std::string& name);

		/** @brief gets the time position of the current animation state
		*
		*  @return time (decimal) that the animation is at.
		*/
		float GetTimePosition(void) const;

		/** @brief sets the animation position
		*
		*  This will cause the animation to jump to a specific frame
		*
		*  @param position where we want to go
		*  @return Void.
		*/
		void SetTimePosition(const float position);

		/** @brief Modifies the animation state by a delta
		*
		*  used to advance the animation along with the game when
		*  it is running
		*
		*  @param dt change in animation position
		*  @return Void.
		*/
		void IncrementTimePosition(const float dt);

		/** @brief gets the currently running animation
		*
		*  @return pointer to the animation
		*/
		const Animation* GetAnimation(void) const;

		/** @brief changes the currently running animation
		*
		*  @param animation the animation to switch to
		*  @return Void.
		*/
		void SetAnimation(Animation* animation);

		const std::string &GetAnimationName(void) const;
		void SetAnimationName(const std::string& name);

		const Animation *GetAnimationByName(void) const;
		void SetAnimationByName(const std::string& name);

		const float &GetTransPosition(void) const;
		void SetTransPosition(const float& tPos);
		
	private:
		std::string m_name;
		//time position to play animation
		float m_timePos;
		std::string m_animname;
        //current animation
        const Animation *m_animation;
		//time to start transitioning
		float m_transPos;
	} Meta(Enable, EnableArrayType, DisplayName( "AnimationState" ));
}