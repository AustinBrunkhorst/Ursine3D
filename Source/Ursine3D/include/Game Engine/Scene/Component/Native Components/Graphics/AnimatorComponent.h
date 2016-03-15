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

namespace ursine
{
    namespace ecs
    {
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

            void OnInitialize(void) override;

            void OnSceneReady(Scene *scene) override;

            void OnSerialize(Json::object &output) const override;
            void OnDeserialize(const Json &input) override;

            // stick this in a system
            void UpdateAnimation(const float dt);

            void Debugging(const AnimationRig& _rig, const std::vector<SMat4>& vec);

            // getter / setter //////////////////////////////////////
            const std::string &GetCurrentState(void) const;
            void SetCurrentState(const std::string &state);

            bool IsPlaying(void) const;
            void SetPlaying(const bool isPlaying);

            bool IsStateChanging(void) const;
            void SetStateChanging(const bool stateChange);

            bool IsDebug(void) const;
            void SetDebug(const bool useDebug);

            bool GetEnableBoneManipulation(void) const;
            void SetEnableBoneManipulation(bool flag);

            float GetTimeScalar(void) const;
            void SetTimeScalar(const float scalar);

            float GetAnimationTimePosition(void) const;
            void SetAnimationTimePosition(const float position);

            // Array of animation states
            ursine::Array<ursine::AnimationState> stArray;
            // Array of state blender
            ursine::Array<ursine::StateBlender> stBlender;

            void UpdateState(AnimationState *currSt, const Animation *currAni,
                AnimationState *futSt, const Animation *futAni, const float& dt, float& transFactor);

            void ChangeState(AnimationState *currSt, AnimationState *futSt,
                             float currloopTimePos, float futloopTimePos, 
                             float currNoloopTimePos, float futNoloopTimePos);

            void GetTransFrmByRatio(AnimationState &state, unsigned int &frameIndex, float ratio);

            StateBlender *GetStateBlenderByNames(const std::string &currst, const std::string &futst);

            // save and load
            // => save both Arrays
            // => when load model, don't just load these, but should also load the animation if it doesn't exist

        private:
            bool m_enableBoneManipulation;
            bool m_playing;
            bool m_debug;
            bool m_changeState;
            bool m_blending;

            float m_speedScalar;

            EntityHandle m_rigRoot;
            std::string m_rig;

            std::string m_curStName;
            std::string m_futStName;
            std::string m_stateName;

            static void recursClearChildren(const std::vector<Handle<Transform>> &children);
            
            void clearChildren(void);

            void createBoneEntities(Transform *parent, AnimationBone *bone);

            void setBoneTransformPointers(Transform *transform, AnimationBone *bone);

            void importRig(void);

            void updateRigTransforms(Component::Handle<Transform> boneTrans, const AnimationBone &boneData);

            void enableDeletionOnEntities(const EntityHandle &entity, bool flag);

            bool loadStateAnimation(AnimationState *state) const;

            // try make state can get animation by EditorResourceField

        } Meta(
            Enable, 
            WhiteListMethods, 
            DisplayName("Animator")
        ) EditorMeta(
                RequiresComponents(typeof(ursine::ecs::Model3D))
        );
    }
}