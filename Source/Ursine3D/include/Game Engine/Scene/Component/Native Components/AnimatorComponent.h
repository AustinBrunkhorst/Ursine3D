/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimatorComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** - Hyung Jun Park - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "Renderable.h"
#include "GfxAPI.h"
#include "AnimationState.h"
#include "AnimationBuilder.h"
#include "Array.h"
#include "Model3DComponent.h"

namespace ursine
{
	namespace ecs
	{		
		class Animator;

		class StateBlender
		{
		public:
			EditorField(
				std::string currentState,
				GetcurrState,
				SetcurrState
				);

			EditorField(
				std::string futureState,
				GetfutState,
				SetfutState
				);

			//Slider
			// blending starting position of current state
			Meta(InputRange(0.0f, 1.0f, 0.01f, "{{value.toPrecision( 2 )}}"))
				EditorField(
					float currtransPos,
					GetcurrTransPosRatio,
					SetcurrTransPosRatio
					);

			// blending end position of future state
			Meta(InputRange(0.0f, 1.0f, 0.01f, "{{value.toPrecision( 2 )}}"))
				EditorField(
					float futtransPos,
					GetfutTransPosRatio,
					SetfutTransPosRatio
					);

			StateBlender(void);

			const std::string &GetcurrState(void) const;
			void SetcurrState(const std::string& cstate);

			const std::string &GetfutState(void) const;
			void SetfutState(const std::string& fstate);

			const float &GetcurrTransPosRatio(void) const;
			void SetcurrTransPosRatio(const float& tPos);

			const float &GetfutTransPosRatio(void) const;
			void SetfutTransPosRatio(const float& tPos);

			const unsigned int &GetcurrTransFrm(void) const;
			void SetcurrTransFrm(const unsigned int& tFrm);

			const unsigned int &GetfutTransFrm(void) const;
			void SetfutTransFrm(const unsigned int& tFrm);

			const StateBlender *GetStateBlenderByNames(const std::string& currst, const std::string& futst);
			
		private:
			// name of current state
			std::string m_currState;
			// name of future state
			std::string m_futState;
			// these reperesent the position of the keyframe 
			// ratio = 1 / total keyframe number.
			// if there is 100 frames, ratio 0.01 represents the fist frame
			float m_ctrnsRate;
			float m_ftrnsRate;
			// frame calculated by rates
			unsigned int m_ctrnsFrm;
			unsigned int m_ftrnsFrm;

		} Meta(
			Enable,
			EnableArrayType,
			DisplayName("State Blender"));

		/////////////////////////////////////////////////////////////

		class Animator : public Component
		{
			NATIVE_COMPONENT;

		public:

			EditorButton(
				ImportAnimation,
				"Import Animation"
			);

			EditorField(
				std::string StateMachineName,
				GetStMachineName,
				SetStMachineName
			);

			EditorField(
				std::string currentState,
				GetCurrentState,
				SetCurrentState
			);

			EditorField(
				std::string futureState,
				GetFutureState,
				SetFutureState
			);

			EditorField(
				std::string animation,
				GetAnimation,
				SetAnimation
			);

			EditorField(
				std::string currentRig,
				GetRig,
				SetRig
			);

			EditorField(
				bool loopAnimation,
				IsLooping,
				SetLooping
			);

			EditorField(
				bool playAnimation,
				IsPlaying,
				SetPlaying
			);

			EditorField(
				bool changeState,
				IsStateChanging,
				SetStateChanging
			);

			EditorField(
				bool renderDebug,
				IsDebug,
				SetDebug
			);
			
			EditorField(
				float timeScalar,
				GetTimeScalar,
				SetTimeScalar
			);
			
			Animator(void);
			~Animator(void);

			Meta(Disable)
			void OnInitialize(void) override;

			// stick this in a system
			void UpdateAnimation(const float dt);

			// getter / setter //////////////////////////////////////
			bool IsPlaying(void) const;
			void SetPlaying(const bool isPlaying);

			bool IsLooping(void) const;
			void SetLooping(const bool isLooping);

			bool IsStateChanging(void) const;
			void SetStateChanging(const bool stateChange);

			bool IsDebug(void) const;
			void SetDebug(const bool useDebug);
			
			float GetTimeScalar(void) const;
			void SetTimeScalar(const float scalar);

			const std::string &GetStMachineName(void) const;
			void SetStMachineName(const std::string &stm);

			const std::string &GetRig(void) const;
			void SetRig(const std::string &rig);
			
			float GetAnimationTimePosition() const;
			void SetAnimationTimePosition(const float position);

			const std::string &GetCurrentState(void) const;
			void SetCurrentState(const std::string &state);

			const std::string &GetStateName(void) const;
			void SetStateName(const std::string &state);

			const std::string &GetFutureState(void) const;
			void SetFutureState(const std::string& name);

			const std::string &GetAnimation(void) const;
			void SetAnimation(const std::string &name);

			// Array of animation states
			ursine::Array<ursine::AnimationState> stArray;
			// Array of state blender
			ursine::Array<ursine::ecs::StateBlender> stBlender;

			void UpdateState(AnimationState* currSt, const Animation* currAni,
				AnimationState* futSt, const Animation* futAni, const float& dt, float& transFactor);

			void GetTransFrmByRatio(AnimationState& state, unsigned int& frameIndex, const float& ratio);

			StateBlender *GetStateBlenderByNames(const std::string& currst, const std::string& futst);
			// save and load
			// => save both Arrays
			// => when load model, don't just load these, but should also load the animation if it doesn't exist

		private:
			bool m_playing;
			bool m_looping;
			bool m_debug;
			bool m_changeState;
			float m_speedScalar;
			std::string m_StateMachineName;
			std::string m_Rig;
			std::string m_currentStateName;
			std::string m_futureStateName;
			std::string m_animationName;
			std::string m_stateName;
			std::vector<Animation*> m_animlist;

			static void recursClearChildren(const std::vector< Handle<Transform> > &children);
			void clearChildren(void);
			void importAnimation(void);
			
		} Meta(
			Enable, 
			RequiresComponents(typeof(ursine::ecs::Model3D)),
			DisplayName("Animator"));
		}
}