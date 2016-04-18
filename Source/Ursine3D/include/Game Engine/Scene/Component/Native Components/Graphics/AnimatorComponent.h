/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimatorComponent.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
**Team Bear King
**?2015 DigiPen Institute of Technology, All Rights Reserved.
**
* *AnimatorComponent.h
**
**Author:
**- Matt Yan - m.yan@digipen.edu
**- Hyung Jun Park - park.hyungjun@digipen.edu
*
**Contributors:
**- <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "Renderable.h"
#include "GfxAPI.h"
#include "AnimationState.h"
#include "AnimationBuilder.h"
#include "Array.h"
#include "Model3DComponent.h"
#include "TransformComponent.h"
#include "AnimationClipData.h"
#include "StateBlender.h"
#include "AnimationEvent.h"

namespace ursine
{
    namespace ecs
    {
        struct AnimatorStateEventArgs : public EventArgs
        {
            std::string state;
            std::string message;

            AnimatorStateEventArgs(AnimationEvent &stEvent)
                : state( stEvent.GetStateName( ) )
                , message( stEvent.GetEventMessage( ) ) { }
        };

        class Animator : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorAnnotate( "Import the rig from the current model." )
            EditorButton(
                ImportRig,
                "Import Rig"
            );

            EditorField(
                std::string currentState,
                GetCurrentState,
                SetCurrentState
            );

            EditorField(
                bool playAnimation,
                IsPlaying,
                SetPlaying
            );

            EditorField(
                bool renderDebug,
                IsDebug,
                SetDebug
            );

            EditorField(
                bool enableBoneManipulation,
                GetEnableBoneManipulation,
                SetEnableBoneManipulation
            );

            EditorField(
                float timeScalar,
                GetTimeScalar,
                SetTimeScalar
            );

            Meta(Enable)
            Animator(void);
            ~Animator(void);

            void OnSceneReady(Scene *scene) override;

            void OnSerialize(Json::object &output) const override;
            void OnDeserialize(const Json &input) override;

            // getter / setter //////////////////////////////////////
            const std::string &GetCurrentState(void) const;
            void SetCurrentState(const std::string &state);

            bool IsPlaying(void) const;
            void SetPlaying(bool isPlaying);

            bool IsDebug(void) const;
            void SetDebug(bool useDebug);

            bool GetEnableBoneManipulation(void) const;
            void SetEnableBoneManipulation(bool flag);

            float GetTimeScalar(void) const;
            void SetTimeScalar(float scalar);

            // Array of animation states
            ursine::Array<ursine::AnimationState> stArray;

            // Array of state blender
            ursine::Array<ursine::StateBlender> stBlender;

            // Array of state events
            ursine::Array<ursine::AnimationEvent> stEvents;

        private:
            friend class AnimatorSystem;

            bool m_enableBoneManipulation;
            bool m_playing;
            bool m_debug;
            bool m_finishEventSent;
            bool m_blending;

            float m_speedScalar;

            // default transition time takes 1 sec this will be used as interpolation factor
            float m_transFactor;

            EntityHandle m_rigRoot;
            std::string m_rig;

            std::string m_curStName;
            std::string m_futStName;
            std::string m_stateName;

            void updateState(AnimationState **currSt, const Animation **currAni,
                            AnimationState **futSt, const Animation **futAni, 
                            //const std::vector<Animation*> &currAnimVec,
                            //const std::vector<Animation*> &futAnimVec,
                            float dt, float &transFactor);

            void animationLoop(AnimationState **currSt, const Animation **currAni,
                            AnimationState **futSt, const Animation **futAni,
                            //const std::vector<Animation*> &currAnimVec,
                            //const std::vector<Animation*> &futAnimVec,
                            float dt, float &transFactor,
                            StateBlender *stateBlender);

            void changeState(AnimationState **currSt, AnimationState **futSt, float &transFactor);

            AnimationState *getAnimationState(const std::string &stateName);

            float getAnimationTimePosition(void);
            void setAnimationTimePosition(float position);

            void getTransFrmByRatio(AnimationState &state, unsigned int &frameIndex, float ratio);

            StateBlender *getStateBlenderByNames(const std::string &currst, const std::string &futst);

            static void recursClearChildren(const std::vector<Handle<Transform>> &children);
            
            void clearChildren(void);

            void createBoneEntities(Transform *parent, AnimationBone *bone);

            void setBoneTransformPointers(Transform *transform, AnimationBone *bone);

            void setRigTransformPointers(void);

            void importRig(void);

            void updateRigTransforms(Component::Handle<Transform> boneTrans, const AnimationBone &boneData);

            void enableDeletionOnEntities(const EntityHandle &entity, bool flag);

            bool loadStateAnimation(AnimationState *state) const;

            void sendAvailableEvents(const std::string &currentState, float currentRatio);

            void resetSentFlagInEvents(const std::string &currentState);

            void setAnimationToFirstFrame(EVENT_HANDLER(Entity));

        } Meta(
            Enable, 
            WhiteListMethods, 
            DisplayName("Animator")
        ) EditorMeta(
                RequiresComponents(typeof(ursine::ecs::Model3D))
        );
    }
}