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
			, m_currtransPos(0.f)
			, m_futtransPos(0.f)
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

		const float &StateBlender::GetcurrTransPos(void) const
		{
			return m_currtransPos;
		}

		void StateBlender::SetcurrTransPos(const float& tPos)
		{
			m_currtransPos = tPos;
			//NOTIFY_COMPONENT_CHANGED("currtransPos", m_currtransPos);
		}

		const float &StateBlender::GetfutTransPos(void) const
		{
			return m_futtransPos;
		}

		void StateBlender::SetfutTransPos(const float& tPos)
		{
			m_futtransPos = 1.0f - m_currtransPos;
			//NOTIFY_COMPONENT_CHANGED("futtransPos", m_futtransPos);
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
			for (auto &x : StateArray)
			{
				if (x.GetName() == m_currentStateName)
					currentState = &x;
				if (x.GetName() == m_futureStateName)
					futureState = &x;
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
			//// selected time of next animation which the blending will ends up
			//float transTime = 1.0f;
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
					// blending = transfactor control X, mix two animation with interp method
					// what do I need?
					// curr transPos - blending starting time position of curr state anim
					// fut transPos - blending end time position of future state anim
					// trans time - time takes for blending
					// to see this, curr->fut animation should be looped.
					transFactor += dt * m_speedScalar;
					if (transFactor >= 1.0f)
					{
						// Change State name
						SetCurrentState(m_futureStateName);
						SetFutureState("");
						// Change actual state too
						currentState = futureState;
						futureState = nullptr;
						// reset transfactor 0
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

			// Create rig Hierarchy tree
		}

		float Animator::GetAnimationTimePosition() const
		{
			for (auto &x : StateArray)
			{
				if (x.GetName() == m_currentStateName)
					return x.GetTimePosition();
			}
			return 0.0f;
		}

		void Animator::SetAnimationTimePosition(const float position)
		{
			for (auto &x : StateArray)
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
			// need to execute AnimationBuilder::LoadAnimation here?
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

		// Question
		// I'm trying to add/remove entity by StateArray.
		// Adding is not hard(Except naming), but how can I remove entity from the blending tree?
		// how can I add/remove entity by updating Array?
		// And how will you control BlendTree for all model?
		// how can I save blendtree? -> by new file format
	}
}