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
#include <Model3DComponent.h>

#include "Notification.h"

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

        Animator::Animator( )
            : BaseComponent( )
            , m_playing( true )
            , m_debug( false )
            , m_changeState( false )
            , m_speedScalar( 1.0f )
            , m_curStName( "" )
            , m_futStName( "" )
            , m_stateName( "" ) { }

        Animator::~Animator(void)
        {
            auto model = GetOwner( )->GetComponent<Model3D>( );

            if (!model)
                return;

            // clean up our model's matrices
            model->clearMatrixPalette( );
        }

        void Animator::OnInitialize(void)
        {
            // clean up our model's matrices
            auto model = GetOwner( )->GetComponent<Model3D>( );

            if (model)
                model->clearMatrixPalette( );
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

        void Animator::UpdateAnimation(const float dt)
        {
            // grab what we need
            AnimationState *currentState = nullptr;
            AnimationState *futureState = nullptr;

            for (auto &x : stArray)
            {
                if (x.GetStateName( ) == "")
                    continue;
                if (x.GetStateName( ) == m_curStName)
                    currentState = &x;
                if (x.GetStateName( ) == m_futStName)
                    futureState = &x;
            }

            if (!currentState)
                return;

            if (!currentState->m_loaded)
                loadStateAnimation( currentState );

            if (futureState && !futureState->m_loaded)
                loadStateAnimation( futureState );

            const Animation *currentAnimation = ( nullptr == currentState ) ? nullptr : currentState->GetAnimation( );
            const Animation *futureAnimation = ( nullptr == futureState ) ? nullptr : futureState->GetAnimation( );

            auto *rig = AnimationBuilder::GetAnimationRigByName( m_rig );

            if (nullptr == currentAnimation || nullptr == rig)
                return;

            if (currentAnimation->GetDesiredBoneCount( ) != rig->GetBoneCount( ))
                return;

            // default transition time takes 1 sec this will be used as interpolation factor
            static float transFactor = 0.0;
            if (nullptr != futureAnimation)
            {
                if (futureAnimation->GetDesiredBoneCount( ) != rig->GetBoneCount( ))
                    return;
            }
            else
                transFactor = 0.0;

            auto &matrixPalette = GetOwner( )->GetComponent<Model3D>( )->getMatrixPalette( );
            std::vector<SMat4> tempVec;

            tempVec.resize( 100 );

            // blending / playing animation should take place in here
            UpdateState( currentState, currentAnimation, futureState, futureAnimation, dt, transFactor );

            // generate the matrices
            AnimationBuilder::GenerateAnimationData(
                currentState,
                futureState,
                rig,
                matrixPalette,
                tempVec,
                ( float )transFactor
            );

            if (!m_rigRoot)
                return;

            // Update the rig transforms
            updateRigTransforms( m_rigRoot->GetTransform( )->GetChild( 0 ), rig->GetBone( 0 ) );

            ////////////////////////////////////////////////////////////////
            // TEMPORARY DEBUG STUFF
            // render the debug data
            if (m_debug)
                Debugging(*rig, tempVec);
        }

        void Animator::Debugging(const AnimationRig& _rig, const std::vector<SMat4>& vec)
        {
            auto *graphics = GetCoreSystem(graphics::GfxAPI);

            URSINE_TODO("Remove this when we implement proper animation clips and stuf")
            std::vector<SVec3> bonePoints;

            bonePoints.resize( 100 );

            auto &hierarchy = _rig.GetHierarchyTable( );
            auto &worldTransform = GetOwner( )->GetTransform( )->GetLocalToWorldMatrix( );

            int maxNodeDistance = 0;

            size_t boneCount = hierarchy.size( );

            // calculate max distance for colors, calculate bone position
            for (size_t x = 0; x < boneCount; ++x)
            {
                // distance
                int distance = 0;
                int walker = hierarchy[x];

                while (walker != -1)
                {
                    walker = hierarchy[walker];
                    distance++;
                }

                if (distance > maxNodeDistance) maxNodeDistance = distance;

                // bone values
                bonePoints[x] = worldTransform.TransformPoint(vec[x].TransformPoint(SVec3(0, 0, 0)));
            }

            // render points
            graphics->DrawingMgr.SetOverdraw(true);

            for (unsigned x = 0; x < boneCount; ++x)
            {
                int distance = 0;
                int walker = hierarchy[x];

                while (walker != -1)
                {
                    walker = hierarchy[walker];
                    distance++;
                }

                float interp = (float)distance / (float)maxNodeDistance;
                //draw points

                graphics->DrawingMgr.SetColor(Color(1  *interp, (1.f - interp), 0, 1));

                if (distance == 0)
                    graphics->DrawingMgr.SetColor(0, 0, 1, 1);

                graphics->DrawingMgr.SetSize(10);
                SVec3 &p = bonePoints[x];
                graphics->DrawingMgr.DrawPoint(p);
            }

            // render lines
            for (size_t x = boneCount - 1; x >= 1; --x)
            {
                SVec3 &p1 = bonePoints[x];
                SVec3 &p2 = bonePoints[hierarchy[x]];

                graphics->DrawingMgr.DrawLine(p1, p2);
            }

            graphics->DrawingMgr.SetOverdraw(false);
        }

        bool Animator::IsPlaying(void) const
        {
            return m_playing;
        }

        void Animator::SetPlaying(const bool isPlaying)
        {
            m_playing = isPlaying;
        }
        
        bool Animator::IsStateChanging(void) const
        {
            return m_changeState;
        }

        void Animator::SetStateChanging(const bool stateChange)
        {
            m_changeState = stateChange;
        }

        bool Animator::IsDebug(void) const
        {
            return m_debug;
        }

        void Animator::SetDebug(const bool useDebug)
        {
            m_debug = useDebug;
        }

        float Animator::GetTimeScalar(void) const
        {
            return m_speedScalar;
        }

        void Animator::SetTimeScalar(const float scalar)
        {
            m_speedScalar = scalar;
        }

        float Animator::GetAnimationTimePosition(void) const
        {
            for (auto &x : stArray)
            {
                if (x.GetStateName( ) == m_curStName)
                    return x.GetTimePosition( );
            }
            return 0.0f;
        }

        void Animator::SetAnimationTimePosition(const float position)
        {
            for (auto &x : stArray)
            {
                if (x.GetStateName( ) == m_curStName)
                {
                    x.SetTimePosition( position );
                    return;
                }
            }
        }

        bool Animator::loadStateAnimation(AnimationState *state) const
        {
            // TODO: Ask Austin
            auto data = loadResource<resources::AnimationClipData>( state->m_clip );

            std::string str;

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
                m_futStName = state;
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

        void Animator::UpdateState(AnimationState* currSt, const Animation* currAni,
                                   AnimationState* futSt, const Animation* futAni, 
                                   const float& dt, float& transFactor)
        {
            if (m_playing)
            {
                currSt->IncrementTimePosition(dt  *m_speedScalar);

                /////////////////////////////////////////////////////
                // this will be applied to all animations that state has
                // const Animation *m_animation; will be changed as std::vector<Animation*>
                /////////////////////////////////////////////////////
                unsigned keyframeCount1 = currAni->GetRigKeyFrameCount( );
                auto &curr_firstFrame = currAni->GetKeyframe(0, 0);
                auto &curr_lastFrame = currAni->GetKeyframe(keyframeCount1 - 1, 0);

                bool bFut = false;
                if (futSt)
                {
                    if (futAni)
                        bFut = true;
                }

                // if there is no future state or future animation, then loop current animation again and again
                if (!bFut)
                {
                    currSt->PlayingAnimation( );// dt  *m_speedScalar);
                }
                // if there is future state and animation
                else
                {
                    // need to check state blender
                    StateBlender *stb = GetStateBlenderByNames(currSt->GetStateName( ), futSt->GetStateName( ));

                    // if user didn't defined state blender, 
                    // then just start blending right now without delay.
                    // (or then play as default state blender which is currTimePos = 1.0f, futTimePos = 0.0f)
                    if (nullptr == stb)
                    {
                        futSt->IncrementTimePosition(dt  *m_speedScalar);
                        
                        transFactor += dt  *m_speedScalar;
                        if (transFactor > 1.0f)
                            transFactor = 1.0f;

                        unsigned keyframeCount2 = futAni->GetRigKeyFrameCount( );
                        auto &fut_firstFrame = futAni->GetKeyframe(0, 0);
                        auto &fut_lastFrame = futAni->GetKeyframe(keyframeCount2 - 1, 0);
                    
                        // if the future state reaches at its last frame
                        if (futSt->GetTimePosition( ) > fut_lastFrame.length)
                        {
                            ChangeState(currSt, futSt
                                , curr_firstFrame.length 
                                , fut_firstFrame.length
                                , curr_lastFrame.length
                                , fut_lastFrame.length );

                            transFactor = 0.0f;
                        }
                    }
                    else
                    {
                        bool bCurrEnd = false;
                        if (currSt->GetTimePosition( ) > curr_lastFrame.length)
                        {
                            currSt->SetTimePosition(curr_lastFrame.length);
                            bCurrEnd = true;
                        }
                    
                        // To check if current state is reached at the same frame as state blender's
                        unsigned int curFrameIndex = 0;
                        GetTransFrmByRatio(*currSt, curFrameIndex, stb->GetcurrTransPosRatio( ));
                        stb->SetcurrTransFrm(curFrameIndex);
                    
                        // Can't check actual frame's length since that keyframe could be dummy value.
                        // so we just check it by index.
                        unsigned index1 = 0, index2 = 0;

                        static bool bBlending = false;
                        if (false == bBlending)
                        {
                            for (unsigned x = 0; x < keyframeCount1 - 1; ++x)
                            {
                                // get the two current keyframes
                                const std::vector<AnimationKeyframe> &f1 = currAni->GetKeyframes(x);
                                const std::vector<AnimationKeyframe> &f2 = currAni->GetKeyframes(x + 1);
                    
                                // check if the current keyframe set holds the time value between them
                                if (f1[0].length <= currSt->GetTimePosition( ) && currSt->GetTimePosition( ) < f2[0].length)
                                    break;

                                ++index1;
                            }

                            index2 = stb->GetcurrTransFrm( );
                            if (index1 == index2)
                            {
                                // Set Trans Frame by Transition Position - fut
                                unsigned int futFrameIndex = 0;
                                GetTransFrmByRatio( *futSt, futFrameIndex, stb->GetfutTransPosRatio( ) );
                                stb->SetfutTransFrm( futFrameIndex );
                                // Set future state's timeposition to chosen frame
                                futSt->SetTimePosition( futAni->GetKeyframe( stb->GetfutTransFrm( ), 0 ).length );
                                // confirm start blending
                                bBlending = true;
                            }
                        }
                    
                        // if the blending is started
                        if (bBlending)
                        {
                            // if blending is true, start transitioning from this state to that state
                            futSt->IncrementTimePosition(dt  *m_speedScalar);

                            transFactor += dt  *m_speedScalar;
                            if (transFactor > 1.0f)
                                transFactor = 1.0f;
                    
                            /////////////////////////////////////////////////////
                            // this will be applied to all animations that state has
                            // const Animation *m_animation; will be changed as std::vector<Animation*>
                            /////////////////////////////////////////////////////
                            unsigned keyframeCount2 = futAni->GetRigKeyFrameCount( );
                            auto &fut_firstFrame = futAni->GetKeyframe(0, 0);
                            auto &fut_lastFrame = futAni->GetKeyframe(keyframeCount2 - 1, 0);
                    
                            if (futSt->GetTimePosition( ) > fut_lastFrame.length)
                            {
                                bBlending = false;

                                ChangeState( currSt, futSt
                                    , curr_firstFrame.length
                                    , futAni->GetKeyframe( stb->GetfutTransFrm( ), 0 ).length
                                    , curr_lastFrame.length
                                    , fut_lastFrame.length );

                                transFactor = 0.0f;
                            }
                        }
                        // if the blending didn't started
                        else
                        {
                            // if current state reached at the end
                            if ( bCurrEnd )
                            {
                                if ( currSt->IsLooping( ) )
                                    currSt->SetTimePosition( curr_firstFrame.length );
                                else
                                    currSt->SetTimePosition( curr_lastFrame.length );
                            }
                        }
                    }
                }
            }
        }
        
        void Animator::ChangeState(AnimationState *currSt, AnimationState *futSt,
                                   float currloopTimePos, float futloopTimePos,
                                   float currNoloopTimePos, float futNoloopTimePos)
        {
            // if change state is checked, change state as future state
            if (m_changeState)
            {
                m_curStName = m_futStName;
                m_futStName = "";

                NOTIFY_COMPONENT_CHANGED("currentState", m_curStName);
                currSt = futSt;
                futSt = nullptr;
            }
            // if not, then it means state will not be changed.
            else
            {
                // if curr state checked looping, then start the whole animation(from curr to fut)
                // all over again.
                if (currSt->IsLooping( ))
                {
                    currSt->SetTimePosition(currloopTimePos);
                    futSt->SetTimePosition(futloopTimePos);
                }
                // else just stop there.
                else
                {
                    currSt->SetTimePosition(currNoloopTimePos);
                    futSt->SetTimePosition(futNoloopTimePos);
                }
            }
        }

        // find the closest animation keyframe of the state, and set a transition position
        void Animator::GetTransFrmByRatio(AnimationState &state, unsigned int &frameIndex, float ratio)
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


        StateBlender *Animator::GetStateBlenderByNames(const std::string &currst, const std::string &futst)
        {
            NotificationConfig config;

            config.type = NOTIFY_WARNING;
            config.header = "Warning";
            config.message = "There is no matching State Blender in the list";
            config.dismissible = true;
            config.duration = TimeSpan::FromSeconds( 5.0f );

            if (currst == "" || futst == "")
            {
                return nullptr;
            }

            for (auto &x : stBlender)
            {
                if (nullptr != x.GetStateBlenderByNames( currst, futst ))
                    return &(x);
            }

            //EditorPostNotification(config);
            return nullptr;
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

            // Add it as a child to our entity with the animator component
            owner->GetTransform( )->AddChildAlreadyInLocal( m_rigRoot->GetTransform( ) );

            m_rigRoot->EnableDeletion( false );
            m_rigRoot->EnableHierarchyChange( false );

            // Iterate through all bones and spawn them as children to the rig root
            auto *rig = AnimationBuilder::GetAnimationRigByName( m_rig );

            createBoneEntities( m_rigRoot->GetTransform( ), rig->GetBone( 0 ) );
        }

        void Animator::createBoneEntities(Transform *parent, const AnimationBone &bone)
        {
            auto world = GetOwner( )->GetWorld( );

            auto boneEntity = world->CreateEntity( bone.GetName( ) );
            auto boneTrans = boneEntity->GetTransform( );

            enableDeletionOnEntities( boneEntity );

            boneTrans->SetLocalPosition( bone.GetTranslation( ) );
            boneTrans->SetLocalRotation( bone.GetRotation( ) );
            boneTrans->SetLocalScale( bone.GetScale( ) );

            parent->AddChildAlreadyInLocal( boneTrans );

            for (auto &child : bone.GetChildren( ))
            {
                createBoneEntities( boneTrans, *child );
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

        void Animator::enableDeletionOnEntities(const ursine::ecs::EntityHandle &entity)
        {
            entity->EnableDeletion( true );
            entity->EnableHierarchyChange( true );

            for (auto &child : *entity->GetChildren( ))
            {
                enableDeletionOnEntities( GetOwner( )->GetWorld( )->GetEntity( child ) );
            }
        }
    }
}