/*----------------------------------------------------------------------------
* *Team Bear King
* *© 2015 DigiPen Institute of Technology, All Rights Reserved.
**
* *AnimatorComponent.cpp
**
**Author:
**- Jordan Ellis - j.ellis@digipen.edu
**- Hyung Jun Park - park.hyungjun@digipen.edu
*
**Contributors:
**- <list in same format as author if applicable>
**--------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AnimatorComponent.h"
#include "Notification.h"
#include "AnimationInfo.h"
#include "Model3DComponent.h"
#include "EntityEvent.h"
#include "Notification.h"
#include "AnimatorSystem.h"
#include "SystemManager.h"

#include <RigComponent.h>
#include <BoneComponent.h>

namespace
{
    const std::string kAnimationListName = "Animation_List";
    const std::string kRigRootName = "Rig_Root";
    const std::string kRigName = "RigName";
}

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(Animator);

        Animator::Animator(void)
            : BaseComponent( )
            , m_playing( true )
            , m_debug( false )
            , m_speedScalar( 1.0f )
            , m_transFactor( 0.0f )
            , m_curStName( "" )
            , m_futStName( "" )
            , m_stateName( "" )
            , m_blending( false )
            , m_finishEventSent( false )
            , m_enableBoneManipulation( false ) { }

        Animator::~Animator(void)
        {
            // enable deletion on the rig root if it exists
            if (m_rigRoot)
                enableDeletionOnEntities( m_rigRoot, true );
        }

        void Animator::OnSceneReady(Scene *scene)
        {
            // If animator isn't playing, and we have a current state,
            // make the rig represent the first frame of animation
            if (!m_playing && !m_curStName.empty( ))
            {
                GetOwner( )->Listener( this )
                    .On( ENTITY_HIERARCHY_SERIALIZED, &Animator::setAnimationToFirstFrame );
            }
        }

        void Animator::OnSerialize(Json::object &output) const
        {
            output[ kRigName ] = m_rig;
        }

        void Animator::OnDeserialize(const Json &input)
        {
            auto &value = input[ kRigName ];

            if (value.is_null( ))
                return;

            m_rig = value.string_value( );
        }

        const std::string &Animator::GetCurrentState(void) const
        {
            return m_curStName;
        }

        void Animator::SetCurrentState(const std::string &state)
        {
            if ("" == m_curStName)
            {
                m_curStName = state;

                NOTIFY_COMPONENT_CHANGED( "currentState", m_curStName );
            }
            else
            {
                m_futStName = state;

                if (m_futStName != m_curStName)
                {
                    // reset the future state's animation event's flags
                    resetSentFlagInEvents( m_futStName );
                }
            }
            m_finishEventSent = false;
        }

        bool Animator::IsPlaying(void) const
        {
            return m_playing;
        }

        void Animator::SetPlaying(bool isPlaying)
        {
            m_playing = isPlaying;
        }

        bool Animator::IsDebug(void) const
        {
            return m_debug;
        }

        void Animator::SetDebug(bool useDebug)
        {
            m_debug = useDebug;
        }

        bool Animator::GetEnableBoneManipulation(void) const
        {
            return m_enableBoneManipulation;
        }

        void Animator::SetEnableBoneManipulation(bool flag)
        {
            m_enableBoneManipulation = flag;
        }

        float Animator::GetTimeScalar(void) const
        {
            return m_speedScalar;
        }

        void Animator::SetTimeScalar(float scalar)
        {
            m_speedScalar = scalar;
        }

        void Animator::updateState(AnimationState **currSt, const Animation **currAni,
                                    AnimationState **futSt, const Animation **futAni,
                                    float dt, float &transFactor)
        {
            // Increment the current state's time marker
            (*currSt)->IncrementTimePosition( dt * m_speedScalar );

            //unsigned keyframeCount1 = (*currAni)->GetRigKeyFrameCount();
            //auto &curr_firstFrame = (*currAni)->GetKeyframe(0, 0);
            //auto &curr_lastFrame = (*currAni)->GetKeyframe(keyframeCount1 - 1, 0);
            //(*currSt)->SetTimePosition( curr_firstFrame.length );
                        
            bool bFut = (*futSt) && (*futAni);

            // if there is no future state or future animation, then loop current animation again and again
            if (!bFut)
            {
                if ((*currSt)->PlayingAnimation() && !m_finishEventSent)
                {
                    if (!(*currSt)->IsLooping( ))
                    {
                        m_finishEventSent = true;

                        GetOwner( )->Dispatch( ENTITY_ANIMATION_FINISH, nullptr );

                        transFactor = 0.0f;
                    }

                    // reset "sent" flag on the state's events
                    resetSentFlagInEvents( (*currSt)->GetStateName( ) );
                }

                return;
            }

            // need to check state blender
            StateBlender *stb = getStateBlenderByNames( (*currSt)->GetStateName( ), (*futSt)->GetStateName( ) );
            
            // if user didn't defined state blender, 
            // else play as default currTimePos = 1.0f, futTimePos = 0.0f
            animationLoop(currSt, currAni, futSt, futAni, dt, transFactor, stb);
        }

        void Animator::animationLoop(AnimationState **currSt, const Animation **currAni,
                                    AnimationState **futSt, const Animation **futAni,
                                    float dt, float &transFactor,
                                    StateBlender *stateBlender)
        {
            unsigned keyframeCount1 = (*currAni)->GetRigKeyFrameCount();
            auto &curr_firstFrame = (*currAni)->GetKeyframe(0, 0);
            auto &curr_lastFrame = (*currAni)->GetKeyframe(keyframeCount1 - 1, 0);

            bool bCurrEnd = false;

            if ((*currSt)->GetTimePosition() > curr_lastFrame.length)
            {
                (*currSt)->SetTimePosition(curr_lastFrame.length);
                bCurrEnd = true;
            }
            
            unsigned keyframeCount2 = (*futAni)->GetRigKeyFrameCount();
            auto &fut_firstFrame = (*futAni)->GetKeyframe(0, 0);
            auto &fut_lastFrame = (*futAni)->GetKeyframe(keyframeCount2 - 1, 0);

            // if there is stateblender, it's little bit complicate
            if (stateBlender)
            {
                // To check if current state is reached at the same frame as state blender's
                unsigned int curFrameIndex = 0;
                getTransFrmByRatio( *(*currSt), curFrameIndex, stateBlender->GetcurrTransPosRatio( ) );
                stateBlender->SetcurrTransFrm( curFrameIndex );
                    
                // Can't check actual frame's length since that keyframe could be dummy value.
                // so we just check it by index.
                unsigned index1 = 0, index2 = 0;

                // Check the frame to approve start blending
                if (!m_blending)
                {
                    for (unsigned x = 0; x < keyframeCount1 - 1; ++x)
                    {
                        // get the two current keyframes
                        const std::vector<AnimationKeyframe> &f1 = (*currAni)->GetKeyframes( x );
                        const std::vector<AnimationKeyframe> &f2 = (*currAni)->GetKeyframes( x + 1 );
                    
                        // check if the current keyframe set holds the time value between them
                        if (f1[0].length <= (*currSt)->GetTimePosition( ) && (*currSt)->GetTimePosition( ) < f2[0].length)
                            break;
                
                        ++index1;
                    }
                
                    index2 = stateBlender->GetcurrTransFrm( );
                    if (index1 >= index2)
                    {
                        // Set Trans Frame by Transition Position - fut
                        unsigned int futFrameIndex = 0;
                        getTransFrmByRatio( *(*futSt), futFrameIndex, stateBlender->GetfutTransPosRatio( ) );
                        stateBlender->SetfutTransFrm( futFrameIndex );

                        // Set future state's timeposition to chosen frame
                        (*futSt)->SetTimePosition( (*futAni)->GetKeyframe(stateBlender->GetfutTransFrm( ), 0 ).length );

                        // approve blending
                        m_blending = true;
                    }
                }
            }
            // if there is no stateblender, future state animation will start at the end of the current state animation
            else
            {
                if (!m_blending && bCurrEnd)
                {
                    (*futSt)->SetTimePosition(fut_firstFrame.length);

                    // approve blending
                    m_blending = true;
                }
            }

            // if the blending is started
            if (m_blending)
            {
                // if blending is true, start transitioning from this state to that state
                (*futSt)->IncrementTimePosition(dt  *m_speedScalar);
                transFactor += dt  *m_speedScalar;
                if (transFactor > 1.0f)
                    transFactor = 1.0f;

                if (stateBlender)
                {                    
                    if ((*futSt)->GetTimePosition( ) > fut_lastFrame.length)
                    {                    
                        changeState(currSt, futSt,
                            curr_firstFrame.length,
                            (*futAni)->GetKeyframe( stateBlender->GetfutTransFrm( ), 0 ).length,
                            curr_lastFrame.length,
                            fut_lastFrame.length,
                            transFactor
                        );
                    }
                }
                else
                {
                    if ((*futSt)->GetTimePosition() > fut_lastFrame.length)
                    {
                        changeState(currSt, futSt,
                            curr_firstFrame.length, 
                            fut_firstFrame.length,
                            curr_lastFrame.length, 
                            fut_lastFrame.length,
                            transFactor
                            );
                    }
                }
            }
            // if the blending didn't started
            else
            {
                // if current state reached at the end
                if (bCurrEnd)
                {
                    if ((*currSt)->IsLooping())
                        (*currSt)->SetTimePosition(curr_firstFrame.length);
                    else
                        (*currSt)->SetTimePosition(curr_lastFrame.length);
                }
            }
        }

        void Animator::changeState(AnimationState **currSt, AnimationState **futSt,
                                   float currloopTimePos, float futloopTimePos,
                                   float currNoloopTimePos, float futNoloopTimePos,
                                   float &transFactor)
        {
            if (!currSt || !(*(currSt)) || !futSt || !(*(futSt)) )
                return;

            m_curStName = m_futStName;
            m_futStName = "";

            (*currSt)->SetTimePosition(currloopTimePos);
            (*currSt) = (*futSt);
            (*futSt) = nullptr;

            transFactor = 0.0f;

            m_blending = false;
        }

        AnimationState *Animator::getAnimationState(const std::string &stateName)
        {
            for (auto &x : stArray)
            {
                if (x.GetStateName( ) == stateName)
                {
                    return &x;
                }
            }

            return nullptr;
        }

        float Animator::getAnimationTimePosition(void)
        {
            auto state = getAnimationState( m_curStName );

            if (state)
                return state->GetTimePosition( );

            return 0.0f;
        }

        void Animator::setAnimationTimePosition(float position)
        {
            auto state = getAnimationState( m_curStName );

            if (state)
                state->SetTimePosition( position );
        }

        // find the closest animation keyframe of the state, and set a transition position
        void Animator::getTransFrmByRatio(AnimationState &state, unsigned int &frameIndex, float ratio)
        {
            unsigned keyframeCount = state.GetAnimation( )->GetRigKeyFrameCount( );
            auto &firstFrame = state.GetAnimation( )->GetKeyframe( 0, 0 );
            auto &lastFrame = state.GetAnimation( )->GetKeyframe( keyframeCount - 1, 0 );
            auto totallength = lastFrame.length - firstFrame.length;
            
            auto delta = 1.0f / totallength;
            auto sec = ratio / delta; // if total 4 second anime, ratio 1 means the momemt of 4 sec

            // find the closest frame
            float diff = totallength;
            for (unsigned int i = 0; i < state.GetAnimation( )->GetRigKeyFrameCount( ); ++i)
            {
                if (fabs( sec - state.GetAnimation( )->GetKeyframe( i, 0 ).length ) < diff)
                {
                    diff = fabs( sec - state.GetAnimation( )->GetKeyframe( i, 0 ).length );
                    frameIndex = i;
                }
            }
        }

        StateBlender *Animator::getStateBlenderByNames(const std::string &currst, const std::string &futst)
        {
            if (currst == "" || futst == "")
            {
                return nullptr;
            }

            for (auto &x : stBlender)
            {
                if (nullptr != x.GetStateBlenderByNames( currst, futst ))
                    return &(x);
            }

            return nullptr;
        }

        bool Animator::loadStateAnimation(AnimationState *state) const
        {
            auto data = loadResource<resources::AnimationClipData>( state->m_clip );
            
            if (data == nullptr)
                return false;

            auto handle = data->GetAnimeHandle( );
            auto info = GetCoreSystem( graphics::GfxAPI )->ResourceMgr.GetAnimInfo( handle );

            state->m_animation = AnimationBuilder::GetAnimationByName( info->name );

            if (state->m_animation == nullptr)
            {
                AnimationBuilder::LoadAnimation( *info );

                state->m_animation = AnimationBuilder::GetAnimationByName( info->name );
            }

            state->m_loaded = true;

            return true;
        }

        void Animator::recursClearChildren(const std::vector< Handle<Transform> > &children)
        {
            for (auto &child : children)
            {
                recursClearChildren(child->GetChildren( ));

                child->GetOwner( )->Delete( );
            }
        }

        void Animator::clearChildren(void)
        {
            recursClearChildren(GetOwner( )->GetTransform( )->GetChildren( ));
        }

#if defined(URSINE_WITH_EDITOR)

        void Animator::ImportRig(void)
        {
            auto owner = GetOwner( );
            auto rigRoot = owner->GetChildByName( kRigRootName );

            // If we currently have a generated rig root
            if (rigRoot)
            {
                NotificationConfig config;

                config.type = NOTIFY_WARNING;
                config.header = "Warning";
                config.message = "This action will delete all of the Rig_Root's children. Continue?";
                config.dismissible = false;
                config.duration = 0;

                NotificationButton yes, no;

                yes.text = "Yes";
                yes.onClick = [=](Notification &notification) {
                    notification.Close( );

                    // Main thread operation
                    Application::PostMainThread( [=] {
                        rigRoot->EnableDeletion( true );
                        rigRoot->Delete( );
                        importRig( );
                    } );
                };

                no.text = "No";
                no.onClick = [=](Notification &notification) {
                    notification.Close( );
                };

                config.buttons = { yes, no };

                EditorPostNotification( config );
                return;
            }

            auto rigName = GetOwner( )->GetComponent<Model3D>( )->GetModelName( );

            if (rigName == "")
            {
                NotificationConfig config;

                config.type = NOTIFY_ERROR;
                config.header = "Error";
                config.message = "The Model3D component doesn't have a model.";
                config.dismissible = true;
                config.duration = TimeSpan::FromSeconds( 5.0f );

                EditorPostNotification( config );

                return;
            }

            Application::PostMainThread( [=] {
                importRig( );
            } );
        }

#endif

        void Animator::importRig(void)
        {
            // Get the rig name from the model
            m_rig = GetOwner( )->GetComponent<Model3D>( )->GetModelName( );

            if (m_rig == "")
                return;

            // Create the "Rig_Root" entity
            auto owner = GetOwner( );
            auto world = owner->GetWorld( );
            m_rigRoot = world->CreateEntity( kRigRootName );

            m_rigRoot->AddComponent<Rig>( );

            // Add it as a child to our entity with the animator component
            owner->GetTransform( )->AddChildAlreadyInLocal( m_rigRoot->GetTransform( ) );

            m_rigRoot->EnableDeletion( false );
            m_rigRoot->EnableHierarchyChange( false );

            // Iterate through all bones and spawn them as children to the rig root
            auto *rig = AnimationBuilder::GetAnimationRigByName( m_rig );

            // if there is no matching rig, it means this mesh is static mesh
            // throw message box
            if (!rig)
            {
                NotificationConfig config;

                config.type = NOTIFY_ERROR;
                config.header = "Error";
                config.message = "The Model is static mesh. There's no matching rig or animation for this.";
                config.dismissible = true;
                config.duration = TimeSpan::FromSeconds(5.0f);

                EditorPostNotification(config);

                return;
            }

            createBoneEntities( m_rigRoot->GetTransform( ), rig->GetBone( 0 ) );
        }

        void Animator::createBoneEntities(Transform *parent, AnimationBone *bone)
        {
            auto world = GetOwner( )->GetWorld( );

            auto boneEntity = world->CreateEntity( bone->GetName( ) );
            auto boneTrans = boneEntity->GetTransform( );

            enableDeletionOnEntities( boneEntity, false );

            boneEntity->AddComponent<Bone>( );

            bone->m_transform = boneTrans;

            parent->AddChildAlreadyInLocal( boneTrans );

            for (auto &child : bone->GetChildren( ))
            {
                createBoneEntities( boneTrans, child );
            }
        }

        void Animator::setBoneTransformPointers(Transform *transform, AnimationBone *bone)
        {
            bone->m_transform = transform;

            for (uint i = 0, j = 0, n = bone->GetChildCount( ); i < n; ++i)
            {
                auto child = transform->GetChild( i )->GetOwner( );

                if (child->HasComponent<Bone>( ))
                    setBoneTransformPointers( child->GetTransform( ), bone->GetChild( j++ ) );
            }
        }

        void Animator::setRigTransformPointers(void)
        {
            if (!m_rigRoot)
                m_rigRoot = GetOwner( )->GetChildByName( kRigRootName );

            if (m_rigRoot)
            {
                if (m_rig == "")
                    return;

                auto *rig = AnimationBuilder::GetAnimationRigByName( m_rig );

                if (!rig)
                    return;

                auto firstBone = m_rigRoot->GetComponentInChildren<Bone>( );

                if (firstBone)
                    setBoneTransformPointers( firstBone->GetOwner( )->GetTransform( ), rig->GetBone( 0 ) );
            }
        }

        void Animator::updateRigTransforms(ursine::ecs::Component::Handle<Transform> boneTrans, const AnimationBone &boneData)
        {
            boneTrans->SetLocalPosition( boneData.GetTranslation( ) );
            boneTrans->SetLocalRotation( boneData.GetRotation( ) );
            boneTrans->SetLocalScale( boneData.GetScale( ) );

            auto &children = boneData.GetChildren( );
            auto count = children.size( );

            for (int i = 0; i < count; ++i)
                updateRigTransforms( boneTrans->GetChild( i ), *children[ i ] );
        }

        void Animator::enableDeletionOnEntities(const EntityHandle &entity, bool flag)
        {
            entity->EnableDeletion( flag );
            entity->EnableHierarchyChange( flag );

            for (auto &child : *entity->GetChildren( ))
            {
                enableDeletionOnEntities( GetOwner( )->GetWorld( )->GetEntity( child ), flag );
            }
        }

        void Animator::sendAvailableEvents(const std::string &currentState, float currentRatio)
        {
            if (currentState == "")
                return;

            for (auto &stEvent : stEvents)
            {
                if (stEvent.GetStateName( ) != currentState)
                    continue;

                if (stEvent.m_sent)
                    continue;

                if (stEvent.GetRatio( ) <= currentRatio)
                {
                    // send the event
                    AnimatorStateEventArgs args( stEvent );

                    GetOwner( )->Dispatch( ENTITY_ANIMATION_STATE_EVENT, &args );

                    if (m_debug)
                        std::cout << "Sending Message: " << args.message << std::endl;

                    // set the "sent" flag
                    stEvent.m_sent = true;
                }
            }
        }

        void Animator::resetSentFlagInEvents(const std::string &currentState)
        {
            if (currentState == "")
                return;

            for (auto &stEvent : stEvents)
            {
                if (stEvent.GetStateName( ) != currentState)
                    continue;

                stEvent.m_sent = false;
            }
        }

        void Animator::setAnimationToFirstFrame(EVENT_HANDLER(Entity))
        {
            EVENT_SENDER(Entity, sender);

            sender->Listener( this )
                .Off( ENTITY_HIERARCHY_SERIALIZED, &Animator::setAnimationToFirstFrame );

            auto state = getAnimationState( m_curStName );

            if (!state)
                return;

            if (!loadStateAnimation( state ))
                return;

            auto animation = state->GetAnimation( );

            auto &curr_firstFrame = animation->GetKeyframe( 0, 0 );

            state->SetTimePosition( curr_firstFrame.length );

            // make sure we have a rig
            if (!m_rigRoot)
            {
                m_rigRoot = GetOwner( )->GetChildByName( kRigRootName );

                if (!m_rigRoot)
                    return;
            }

            // Update this animator once
            m_playing = true;

            auto animatorSystem = GetOwner( )->GetWorld( )->GetEntitySystem<AnimatorSystem>( );

            animatorSystem->updateAnimator( this, Application::Instance->GetDeltaTime( ) );

            m_playing = false;
        }
    }
}
