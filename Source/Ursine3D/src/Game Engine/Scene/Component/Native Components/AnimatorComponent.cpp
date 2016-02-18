﻿/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimatorComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AnimatorComponent.h"
#include "Notification.h"
#include "../Serialization/AnimationInfo.h"
#include <Game Engine/Scene/Component/Native Components/Model3DComponent.h>

typedef ursine::graphics::ufmt_loader::AnimInfo AniInfo;

namespace ursine
{
	namespace ecs
	{
		StateBlender::StateBlender(void)
			: m_currState("")
			, m_futState("")
			, m_currtransPos(1.f)
			, m_futtransPos(0.f)
			, m_currtransFrm(0)
			, m_futtransFrm(0)
		{}

		const std::string &StateBlender::GetcurrState(void) const
		{
			return m_currState;
		}

		void StateBlender::SetcurrState(const std::string& cstate)
		{
			m_currState = cstate;
		}

		const std::string &StateBlender::GetfutState(void) const
		{
			return m_futState;
		}

		void StateBlender::SetfutState(const std::string& fstate)
		{
			m_futState = fstate;
		}

		const float &StateBlender::GetcurrTransPosRatio(void) const
		{
			return m_currtransPos;
		}

		void StateBlender::SetcurrTransPosRatio(const float& tPos)
		{
			m_currtransPos = tPos;
		}

		const float &StateBlender::GetfutTransPosRatio(void) const
		{
			return m_futtransPos;
		}

		void StateBlender::SetfutTransPosRatio(const float& tPos)
		{
			m_futtransPos = tPos;
		}

		const unsigned int &StateBlender::GetcurrTransFrm(void) const
		{
			return m_currtransFrm;
		}

		void StateBlender::SetcurrTransFrm(const unsigned int& tFrm)
		{
			m_currtransFrm = tFrm;
		}

		const unsigned int &StateBlender::GetfutTransFrm(void) const
		{
			return m_futtransFrm;
		}

		void StateBlender::SetfutTransFrm(const unsigned int& tFrm)
		{
			m_futtransFrm;
		}

		const StateBlender *StateBlender::GetStateBlenderByNames(const std::string& currst, const std::string& futst)
		{
			if (currst == m_currState && futst == m_futState)
				return this;
			return nullptr;
		}
		
		NATIVE_COMPONENT_DEFINITION(Animator);

		Animator::Animator()
			: BaseComponent()
			, m_playing(true)
			, m_looping(true)
			, m_debug(false)
			, m_changeState(false)
			, m_speedScalar(1.0f)
			, m_Rig("")
			, m_currentStateName("")
			, m_futureStateName("")
			, m_animationName("")
			, m_stateName("")
			, m_animlist(0)
		{
		}

		Animator::~Animator()
		{
			m_animlist.clear();
		}

		void Animator::OnInitialize(void)
		{
			// clean up our model's matrices
			auto &matrixPalette = GetOwner()->GetComponent<Model3D>()->GetMatrixPalette();
			for (auto &x : matrixPalette)
			{
				x = SMat4::Identity();
			}

			auto *gfx = GetCoreSystem(graphics::GfxAPI);
			auto *world = GetOwner()->GetWorld();
			auto *animListEntity = world->CreateEntity("Animation List");
			auto *blendTreeEntity = world->CreateEntity("Blending Tree");
		}

		void Animator::UpdateAnimation(const float dt)
		{
			URSINE_TODO("Try playing every animation states");

#if defined(URSINE_WITH_EDITOR)
			// Could update the situation of Blend Tree Entity here
#endif
			// grab what we need
			AnimationState *currentState = nullptr;
			AnimationState *futureState = nullptr;
			for (auto &x : stArray)
			{
				if (x.GetName() == "")
					continue;
				if (x.GetName() == m_currentStateName)
					currentState = &x;
				if (x.GetName() == m_futureStateName)
					futureState = &x;
			}

			if (!currentState)
				return;

			const Animation *currentAnimation = (nullptr == currentState) ? nullptr : currentState->GetAnimation();
			const Animation *futureAnimation = (nullptr == futureState) ? nullptr : futureState->GetAnimation();
			auto *rig = AnimationBuilder::GetAnimationRigByName(m_Rig);

			if (nullptr == currentAnimation || nullptr == rig)
				return;

			if (currentAnimation->GetDesiredBoneCount() != rig->GetBoneCount())
				return;

			// default transition time takes 1 sec this will be used as interpolation factor
			static float transFactor = 0.0;
			//// selected time of next animation which the blending will ends up
			//float transTime = 1.0f;
			if (nullptr != futureAnimation)
			{
				if (futureAnimation->GetDesiredBoneCount() != rig->GetBoneCount())
					return;
			}
			else
				transFactor = 0.0;

			auto &matrixPalette = GetOwner()->GetComponent<Model3D>()->GetMatrixPalette();
			std::vector<SMat4> tempVec(100);

			// blending / playing animation should take place in here
			UpdateState(currentState, currentAnimation, futureState, futureAnimation, dt, transFactor);

			// generate the matrices
			AnimationBuilder::GenerateAnimationData(
				currentState,
				futureState,
				rig,
				matrixPalette,
				tempVec,
				(float)transFactor
				);

			//////////////////////////////////////////////////////////////////
			//// TEMPORARY DEBUG STUFF
			//// render the debug data
			//if ( m_debug )
			//{
			//    auto *graphics = GetCoreSystem( graphics::GfxAPI );
			//    URSINE_TODO( "Remove this when we implement proper animation clips and stuf" )
			//        std::vector<SVec3> bonePoints( 100 );
			//    auto &hierarchy = rig->GetHierarchyTable( );
			//    auto &worldTransform = GetOwner( )->GetTransform( )->GetLocalToWorldMatrix( );
			//
			//    int maxNodeDistance = 0;
			//
			//    size_t boneCount = hierarchy.size( );
			//
			//    // calculate max distance for colors, calculate bone position
			//    for (size_t x = 0; x < boneCount; ++x)
			//    {
			//        // distance
			//        int distance = 0;
			//        int walker = hierarchy[ x ];
			//
			//        while ( walker != -1 )
			//        {
			//            walker = hierarchy[ walker ];
			//            distance++;
			//        }
			//
			//        if ( distance > maxNodeDistance ) maxNodeDistance = distance;
			//
			//        // bone values
			//        bonePoints[ x ] = worldTransform.TransformPoint( tempVec[ x ].TransformPoint( SVec3( 0, 0, 0 ) ) );
			//    }
			//
			//    // render points
			//    graphics->DrawingMgr.SetOverdraw( true );
			//    for ( unsigned x = 0; x < boneCount; ++x )
			//    {
			//        int distance = 0;
			//        int walker = hierarchy[ x ];
			//
			//        while ( walker != -1 )
			//        {
			//            walker = hierarchy[ walker ];
			//            distance++;
			//        }
			//
			//        float interp = (float)distance / (float)maxNodeDistance;
			//        //draw points
			//        
			//        graphics->DrawingMgr.SetColor( Color( 1 * interp, (1.f - interp), 0, 1 ) );
			//
			//        if ( distance == 0 )
			//            graphics->DrawingMgr.SetColor( 0, 0, 1, 1 );
			//
			//        graphics->DrawingMgr.SetSize( 10 );
			//        SVec3 &p = bonePoints[ x ];
			//        graphics->DrawingMgr.DrawPoint( p );
			//    }
			//
			//    // render lines
			//    for ( size_t x = boneCount - 1; x >= 1; --x )
			//    {
			//        SVec3 &p1 = bonePoints[ x ];
			//        SVec3 &p2 = bonePoints[ hierarchy[ x ] ];
			//
			//        graphics->DrawingMgr.DrawLine( p1, p2 );
			//    }
			//
			//    graphics->DrawingMgr.SetOverdraw( false );
			//}            
		}

		bool Animator::IsPlaying(void) const
		{
			return m_playing;
		}

		void Animator::SetPlaying(const bool isPlaying)
		{
			m_playing = isPlaying;
		}

		bool Animator::IsLooping(void) const
		{
			return m_looping;
		}

		void Animator::SetLooping(const bool isLooping)
		{
			m_looping = isLooping;
		}

		bool Animator::IsStateChanging(void) const
		{
			return m_changeState;
		}

		void Animator::SetStateChanging(const bool stateChange)
		{
			m_changeState = stateChange;
		}

		bool Animator::IsDebug() const
		{
			return m_debug;
		}

		void Animator::SetDebug(const bool useDebug)
		{
			m_debug = useDebug;
		}

		float Animator::GetTimeScalar() const
		{
			return m_speedScalar;
		}

		void Animator::SetTimeScalar(const float scalar)
		{
			m_speedScalar = scalar;
		}
		
		const std::string &Animator::GetAnimation(void) const
		{
			return m_animationName;
		}
		
		void Animator::SetAnimation(const std::string& name)
		{
			m_animationName = name;
		}

		const std::string &Animator::GetRig() const
		{
			return m_Rig;
		}

		void Animator::SetRig(const std::string &rig)
		{
			m_Rig = rig;

			// Create rig Hierarchy tree
		}

		float Animator::GetAnimationTimePosition() const
		{
			for (auto &x : stArray)
			{
				if (x.GetName() == m_currentStateName)
					return x.GetTimePosition();
			}
			return 0.0f;
		}

		void Animator::SetAnimationTimePosition(const float position)
		{
			for (auto &x : stArray)
			{
				if (x.GetName() == m_currentStateName)
				{
					x.SetTimePosition(position);
					return;
				}
			}
		}

		const std::string& Animator::GetCurrentState(void) const
		{
			return m_currentStateName;
		}

		void Animator::SetCurrentState(const std::string &state)
		{
			m_currentStateName = state;
			NOTIFY_COMPONENT_CHANGED("currentState", m_currentStateName);
		}

		const std::string &Animator::GetFutureState(void) const
		{
			return m_futureStateName;
		}

		void Animator::SetFutureState(const std::string& name)
		{
			m_futureStateName = name;
			NOTIFY_COMPONENT_CHANGED("futureState", m_futureStateName);
		}

		const std::string& Animator::GetStateName(void) const
		{
			return m_stateName;
		}

		void Animator::SetStateName(const std::string &state)
		{
			m_stateName = state;
		}

		void Animator::ImportAnimation(void)
		{
			auto owner = GetOwner();
			auto *children = owner->GetChildren();

			if (children->size() > 0)
			{
				NotificationConfig config;
				config.type = NOTIFY_WARNING;
				config.header = "Warning";
				config.message = "This action will delete all of the FBXSceneRootNode's children. Continue?";
				config.dismissible = false;
				config.duration = 0;

				NotificationButton yes, no;
				yes.text = "Yes";
				yes.onClick = [=](Notification &notification) {
					notification.Close();

					// Main thread operation
					Timer::Create(0).Completed([=] {
						clearChildren();
						importAnimation();
					});
				};
				no.text = "No";
				no.onClick = [=](Notification &notification) {
					notification.Close();
				};
				config.buttons = { yes, no };
				EditorPostNotification(config);
			}
			else
			{
				// Main thread operation
				Timer::Create(0).Completed([=] {
					importAnimation();
				});
			}
		}

		void Animator::recursClearChildren(const std::vector< Handle<Transform> > &children)
		{
			for (auto &child : children)
			{
				recursClearChildren(child->GetChildren());

				child->GetOwner()->Delete();
			}
		}

		void Animator::clearChildren(void)
		{
			recursClearChildren(GetOwner()->GetTransform()->GetChildren());
		}

		// import animation to the current state
		// if I get animation builder here, how can I put animation to the state?
		void Animator::importAnimation(void)
		{
			std::string janiFileName("Assets/Animations/");
			janiFileName += m_animationName + ".jani";
			HANDLE hFile_ani = CreateFile(janiFileName.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
			if (!hFile_ani)
				return;

			AniInfo ufmt_ani;
			// Serialize in model and animation
			UAssert(true == ufmt_ani.SerializeIn(hFile_ani), "Fail to serialize jani file.", janiFileName.c_str());

			unsigned animationIndex = 0;
			// Check if there is same animation already
			const Animation* checker = AnimationBuilder::GetAnimationByName(m_animationName);
			if(nullptr == checker)
				animationIndex = AnimationBuilder::LoadAnimation(ufmt_ani, m_animationName);
			CloseHandle(hFile_ani);

			// Check if the animation is in animlist, push back if not
			bool bExist = false;
			for (auto &x : m_animlist)
			{
				if (m_animationName == x->GetName())
				{
					bExist = true;
					break;
				}
			}

			if (!bExist)
			{
				// add to animlist
				m_animlist.push_back(AnimationBuilder::GetAnimationByName(m_animationName));

				auto *gfx = GetCoreSystem(graphics::GfxAPI);
				auto *world = GetOwner()->GetWorld();
				auto *animList = world->GetEntityFromName("Animation List");
				auto *alTrans = animList->GetTransform();
				auto *newEntity = world->CreateEntity(m_animationName.c_str());
				alTrans->AddChild(newEntity->GetTransform());
			}
		}

		void Animator::UpdateState(AnimationState* currSt, const Animation* currAni,
			AnimationState* futSt, const Animation* futAni, const float& dt, float& transFactor)
		{
			if (m_playing)
			{
				currSt->IncrementTimePosition(dt * m_speedScalar);
				
				/////////////////////////////////////////////////////
				// this will be applied to all animations that state has
				// const Animation *m_animation; will be changed as std::vector<Animation*>
				/////////////////////////////////////////////////////
				unsigned keyframeCount1 = currAni->GetRigKeyFrameCount();
				auto &curr_firstFrame = currAni->GetKeyframe(0, 0);
				auto &curr_lastFrame = currAni->GetKeyframe(keyframeCount1 - 1, 0);

				static bool bBlending = false;
				bool bFut = false;
				if (futSt)
				{
					if (futAni)
						bFut = true;
				}

				// if current State is reached at the end
				if (currSt->GetTimePosition() > curr_lastFrame.length)
				{
					if (m_looping)
						currSt->SetTimePosition(curr_firstFrame.length); 
					else
						currSt->SetTimePosition(curr_lastFrame.length);
				}

				// if there is future state and animation
				if (bFut)
				{
					// need to check state blender
					StateBlender* stb = GetStateBlenderByNames(currSt->GetName(), futSt->GetName());
					if (nullptr == stb)
					{
						unsigned keyframeCount2 = futAni->GetRigKeyFrameCount();
						auto &fut_firstFrame = futAni->GetKeyframe(0, 0);
						auto &fut_lastFrame = futAni->GetKeyframe(keyframeCount2 - 1, 0);
						futSt->IncrementTimePosition(dt * m_speedScalar);
						transFactor += dt * m_speedScalar;
						if (transFactor > 1.0f)
							transFactor = 1.0f;

						if (futSt->GetTimePosition() > fut_lastFrame.length)
						{
							if (m_changeState)
							{
								SetCurrentState(m_futureStateName);
								SetFutureState("");
								currSt = futSt;
								futSt = nullptr;
							}
							else
							{
								if (m_looping)
								{
									currSt->SetTimePosition(curr_firstFrame.length);
									futSt->SetTimePosition(fut_firstFrame.length);
								}
								else
								{
									currSt->SetTimePosition(curr_lastFrame.length);
									futSt->SetTimePosition(fut_lastFrame.length);
								}
							}
						}
					}
					else
					{
						unsigned int curFrameIndex = 0;
						SetTransFrame(*currSt, curFrameIndex, stb->GetcurrTransPosRatio());
						stb->SetfutTransFrm(curFrameIndex);

						unsigned index1 = 0, index2 = 0;
						for (unsigned x = 0; x < keyframeCount1 - 1; ++x)
						{
							// get the two current keyframes
							const std::vector<AnimationKeyframe> &f1 = currAni->GetKeyframes(x);
							const std::vector<AnimationKeyframe> &f2 = currAni->GetKeyframes(x + 1);
					
							// check if the current keyframe set holds the time value between them
							if (f1[0].length <= currSt->GetTimePosition() && currSt->GetTimePosition() < f2[0].length)
								break;
							++index1;
						}
						index2 = stb->GetcurrTransFrm();
						if (index1 == index2)
							bBlending = true;
					
						// Set Trans Frame by Transition Position - curr
						if (bBlending)
						{
							// if blending is true, start transitioning from this state to that state
							futSt->IncrementTimePosition(dt * m_speedScalar);
							transFactor += dt * m_speedScalar;
							if (transFactor > 1.0f)
								transFactor = 1.0f;

							// Set Trans Frame by Transition Position - fut
							unsigned int futFrameIndex = 0;
							SetTransFrame(*futSt, futFrameIndex, stb->GetfutTransPosRatio());
							stb->SetfutTransFrm(futFrameIndex);
					
							/////////////////////////////////////////////////////
							// this will be applied to all animations that state has
							// const Animation *m_animation; will be changed as std::vector<Animation*>
							/////////////////////////////////////////////////////
							unsigned keyframeCount2 = futAni->GetRigKeyFrameCount();
							auto &fut_firstFrame = futAni->GetKeyframe(0, 0);
							auto &fut_lastFrame = futAni->GetKeyframe(keyframeCount2 - 1, 0);
					
							if (futSt->GetTimePosition() > fut_lastFrame.length)
							{
								bBlending = false;
								transFactor = 0.0f;
								if (m_changeState)
								{
									SetCurrentState(m_futureStateName);
									SetFutureState("");
									currSt = futSt;
									futSt = nullptr;
								}
								else
								{
									if (m_looping)
										futSt->SetTimePosition(futAni->GetKeyframe(stb->GetfutTransFrm(), 0).length);
									else
										futSt->SetTimePosition(fut_lastFrame.length);
								}
							}
						}
					}
				}
			}
		}

		// find the closest animation keyframe of the state, and set a transition position
		void Animator::SetTransFrame(AnimationState& state, unsigned int& frameIndex, const float& ratio)
		{
			unsigned keyframeCount = state.GetAnimation()->GetRigKeyFrameCount();
			auto &firstFrame = state.GetAnimation()->GetKeyframe(0, 0);
			auto &lastFrame = state.GetAnimation()->GetKeyframe(keyframeCount - 1, 0);
			auto totallength = lastFrame.length - firstFrame.length;
			auto rate = 0.f;

			const AnimationKeyframe* closest = nullptr;
			float cumul_length = 0.0f;
			float diff = 1.0f;
			// this wouldn't work properly unless keyframe length didn't recorded properly.
			// then how can I get frame by ratio?
			for (auto& x : state.GetAnimation()->GetKeyframes(0))
			{
				cumul_length += x.length;
				float curr_rate = cumul_length / totallength;
				if (ratio - curr_rate < diff)
				{
					diff = ratio - curr_rate;
					closest = &x;
				}
				++frameIndex;
			}
			state.SetTransPosition(closest->length);
		}

		StateBlender *Animator::GetStateBlenderByNames(const std::string& currst, const std::string& futst)
		{
			NotificationConfig config;
			config.type = NOTIFY_WARNING;
			config.header = "Warning";
			config.message = "There is no matching State Blender in the list";
			config.dismissible = false;
			config.duration = 0;

			if (currst == "" || futst == "")
			{
				//EditorPostNotification(config);
				return nullptr;
			}

			for (auto &x : stBlender)
			{
				if (nullptr != x.GetStateBlenderByNames(currst, futst))
					return &(x);
			}

			//EditorPostNotification(config);
			return nullptr;
		}


		// Question
		// I'm trying to add/remove entity by StateArray.
		// Adding is not hard(Except naming), but how can I remove entity from the blending tree?
		// how can I add/remove entity when I add/remove Array?
		// And how will you control BlendTree for all model?
		// how can I save blendtree? -> by new file format
	}
}