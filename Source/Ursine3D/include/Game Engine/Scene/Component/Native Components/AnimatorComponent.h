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
            EditorMeta(InputRange(0.0f, 1.0f, 0.01f, "{{value.toPrecision( 2 )}}"))
            EditorField(
                float currtransPos,
                GetcurrTransPosRatio,
                SetcurrTransPosRatio
            );

            //Slider
            // blending end position of future state
            EditorMeta(InputRange(0.0f, 1.0f, 0.01f, "{{value.toPrecision( 2 )}}"))
            EditorField(
                float futtransPos,
                GetfutTransPosRatio,
                SetfutTransPosRatio
            );

            Meta(Enable)
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
            WhiteListMethods,
            EnableArrayType,
            DisplayName("State Blender")
        );

        class Animator : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorButton(
                ImportAnimation,
                "Import Animation"
            );

            EditorButton(
                ImportRig,
                "Import Rig"
            );

            // Get animation as a resource not by name
            EditorResourceField(
                ursine::resources::AnimationClipData,
                clip,
                GetClip,
                SetClip
            );

            EditorField(
                std::string currentState,
                GetCurrentState,
                SetCurrentState
            );

            EditorField(
                std::string currentRig,
                GetRig,
                SetRig
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

            Meta(Enable)
            Animator(void);
            ~Animator(void);

            void OnInitialize(void) override;

            // stick this in a system
            void UpdateAnimation(const float dt);

            void Debugging(const AnimationRig& _rig, const std::vector<SMat4>& vec);

            // getter / setter //////////////////////////////////////
            bool IsPlaying(void) const;
            void SetPlaying(const bool isPlaying);

            bool IsStateChanging(void) const;
            void SetStateChanging(const bool stateChange);

            bool IsDebug(void) const;
            void SetDebug(const bool useDebug);
            
            float GetTimeScalar(void) const;
            void SetTimeScalar(const float scalar);

            const std::string &GetRig(void) const;
            void SetRig(const std::string &rig);
            
            float GetAnimationTimePosition(void) const;
            void SetAnimationTimePosition(const float position);

            const resources::ResourceReference &GetClip(void) const;
            void SetClip(const resources::ResourceReference &clip);
            void invalidateClip(void);

            const std::string &GetCurrentState(void) const;
            void SetCurrentState(const std::string &state);

            // Array of animation states
            ursine::Array<ursine::AnimationState> stArray;
            // Array of state blender
            ursine::Array<ursine::ecs::StateBlender> stBlender;

            void UpdateState(AnimationState *currSt, const Animation *currAni,
                AnimationState *futSt, const Animation *futAni, const float& dt, float& transFactor);

            void ChangeState(AnimationState *currSt, AnimationState *futSt,
                const float &currloopTimePos, const float &futloopTimePos, 
                const float &currNoloopTimePos, const float &futNoloopTimePos);

            void GetTransFrmByRatio(AnimationState& state, unsigned int& frameIndex, const float& ratio);

            StateBlender *GetStateBlenderByNames(const std::string& currst, const std::string& futst);
            // save and load
            // => save both Arrays
            // => when load model, don't just load these, but should also load the animation if it doesn't exist
            
        private:
            bool m_playing;
            bool m_debug;
            bool m_changeState;
            float m_speedScalar;

            std::string m_rig;
            EntityHandle m_rigRoot;
            std::string m_curStName;
            std::string m_futStName;
            std::string m_animationName;
            std::string m_stateName;
            std::vector<Animation*> m_animlist;

            resources::ResourceReference m_clipResource;

            static void recursClearChildren(const std::vector<Handle<Transform>> &children);
            void clearChildren(void);
            void importAnimation(void);
            void createBoneEntities(ursine::ecs::Transform *parent, const AnimationBone &bone);

            void importRig(void);

            void updateRigTransforms(ursine::ecs::Component::Handle<Transform> boneTrans, const AnimationBone &boneData);

            void enableDeletionOnEntities(const ursine::ecs::EntityHandle &entity);

        } Meta(
            Enable, 
            WhiteListMethods, 
            DisplayName("Animator")
        ) EditorMeta(
                RequiresComponents(typeof(ursine::ecs::Model3D))
        );
    }
}