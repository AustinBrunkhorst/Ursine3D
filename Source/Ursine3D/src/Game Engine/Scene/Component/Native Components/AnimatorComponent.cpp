/* ----------------------------------------------------------------------------
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
#include <Game Engine/Scene/Component/Native Components/Model3DComponent.h>

namespace ursine
{
	namespace ecs
	{
		NATIVE_COMPONENT_DEFINITION(Animator);

		Animator::Animator()
			: BaseComponent()
			, m_playing(false)
			, m_looping(true)
			, m_debug(false)
			, m_speedScalar(1.0f)
			, m_Rig("")
			, m_currentStateName("")
			, m_futureStateName("")
			, m_animationName("")
			, m_stateName("")
		{
		}

		Animator::~Animator()
		{
			//m_states.clear();
		}

		void Animator::OnInitialize(void)
		{
			// clean up our model's matrices
			auto &matrixPalette = GetOwner()->GetComponent<Model3D>()->GetMatrixPalette();
			for (auto &x : matrixPalette)
			{
				x = SMat4::Identity();
			}
		}

		void Animator::UpdateAnimation(const float dt)
		{
			URSINE_TODO("Try playing every animation states");

			// grab what we need
			AnimationState *currentState = nullptr;
			AnimationState *futureState = nullptr;
			for (auto iter = m_stateArray.begin(); iter != m_stateArray.end(); ++iter)
			{
				if ((*iter).GetName() == m_currentStateName)
					currentState = &(*iter);
				if ((*iter).GetName() == m_futureStateName)
					futureState = &(*iter);
			}
			
			if (!currentState)
				return;
			
			const Animation *currentAnimation = (nullptr == currentState)? nullptr : currentState->GetAnimation();
			const Animation *futrueAnimation = (nullptr == futureState)? nullptr : futureState->GetAnimation();
			auto *rig = AnimationBuilder::GetAnimationRigByName(m_Rig);
			
			if ( nullptr == currentAnimation || nullptr == rig )
				return;
			
			if (currentAnimation->GetDesiredBoneCount() != rig->GetBoneCount())
				return;
			
			// default transition time takes 1 sec this will be used as interpolation factor
			static float transFactor = 0.0f;
			// selected time of next animation which the blending will ends up
			float transTime = 1.0f;
			if (nullptr != futrueAnimation)
			{
				if (futrueAnimation->GetDesiredBoneCount() != rig->GetBoneCount())
					return;
			}
			
			auto &matrixPalette = GetOwner()->GetComponent<Model3D>()->GetMatrixPalette();
			std::vector<SMat4> tempVec(100);
			
			// update time
			if (m_playing)
			{
				unsigned keyframeCount = currentAnimation->GetRigKeyFrameCount();
				auto &curr_firstFrame = currentAnimation->GetKeyframe(0, 0);
				auto &curr_lastFrame = currentAnimation->GetKeyframe(keyframeCount - 1, 0);
			
				currentState->IncrementTimePosition(dt * m_speedScalar);
				if (nullptr != futrueAnimation && currentAnimation != futrueAnimation)
				{
					futureState->IncrementTimePosition(dt * m_speedScalar);
					transFactor += 0.05f * m_speedScalar; // / transTime;
														  // if there is future animation
					if (transFactor >= 1.0f)
					{
						m_currentStateName = m_futureStateName;
						SetCurrentState(m_currentStateName);
						SetFutureState("");
						m_futureStateName = "";
						transFactor = 0.f;
					}
				}
			
				// if current state reached at the end of its frame
				if (currentState->GetTimePosition() > curr_lastFrame.length)
				{
					// if we need to loop, go back to 0, maybe the first frame time?
					if (m_looping)
						currentState->SetTimePosition(curr_firstFrame.length);
					else
						currentState->SetTimePosition(curr_lastFrame.length);
				}
			}
			
			// generate the matrices
			AnimationBuilder::GenerateAnimationData(
				currentState,
				futureState,
				rig,
				matrixPalette,
				tempVec,
				transFactor
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
		}

		float Animator::GetAnimationTimePosition() const
		{
			for (auto &x : m_stateArray)
			{
				if (x.GetName() == m_currentStateName)
					return x.GetTimePosition();
			}
			return 0.0f;
		}

		void Animator::SetAnimationTimePosition(const float position)
		{
			for (auto &x : m_stateArray)
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
	}
}