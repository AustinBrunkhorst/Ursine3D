/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ShaderBufferManager.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "Renderable.h"
#include "TextureData.h"
#include "Model3DComponent.h"

namespace ursine
{
    namespace ecs
    {
        class ModelFragmenter : public Component
        {
            NATIVE_COMPONENT;

            friend class RenderSystem;

        public:
            EditorButton(
                Play,
                "Play Fragmentation"
            );

            EditorButton(
                Pause,
                "Pause Fragmentation"
            );

            EditorButton(
                Restart,
                "Restart Fragmentation"
            );

            EditorField(
                bool active,
                GetIsActive,
                SetIsActive
            );

            EditorField(
                bool playingBackwards,
                GetPlayInReverse,
                SetPlayInReverse
            );

            EditorResourceField(
                ursine::resources::TextureData,
                texture,
                GetTexture,
                SetTexture
            );

            EditorField(
                Color colorTint,
                GetColor,
                SetColor
            );

            EditorMeta(InputRange(0.0f, 1.0f, 0.001f, "{{value.toPrecision( 3 )}}"))
            EditorField(
                float timeSlider,
                GetTimeSliderValue,
                SetTimeSliderValue
            );

            EditorField(
                float time,
                GetTime,
                SetTime
            );

            EditorField(
                float maxTime,
                GetMaxTime,
                SetMaxTime
            );

            EditorField(
                float verticalForce,
                GetVerticalForce,
                SetVerticalForce
            );

            EditorField(
                float horizontalForce,
                GetHorizontalForce,
                SetHorizontalForce
            );

            EditorField(
                float outwardForce,
                GetOutwardForce,
                SetOutwardForce
            );

            EditorField(
                float gravityForce,
                GetGravityForce,
                SetGravityForce
            );

            EditorField(
                float randomForce,
                GetRandomForce,
                SetRandomForce
            );

            EditorField(
                float pulseSpeed,
                GetPulseSpeed,
                SetPulseSpeed
            );

            EditorField(
                float fadeAmount,
                GetFadeAmount,
                SetFadeAmount
            );

            EditorField(
                float transparencyThreshold,
                GetTransparencyThreshold,
                SetTransparencyThreshold
            );

            EditorField(
                float normalOffset,
                GetNormalOffset,
                SetNormalOffset
            );

            EditorField(
                float spinScalar,
                GetSpinScalar,
                SetSpinScalar
            );

            EditorField(
                Vec2 textureOffset,
                GetTextureUV,
                SetTextureUV
            );

            Meta(Enable)
            ModelFragmenter(void);
            ~ModelFragmenter(void);

            void OnInitialize(void) override;
            void OnSceneReady(Scene *scene) override;

            bool GetIsActive(void) const;
            void SetIsActive(bool isActive);

            const resources::ResourceReference &GetTexture(void) const;
            void SetTexture(const resources::ResourceReference &texture);

            float GetTime(void) const; 
            void SetTime(float value);

            float GetMaxTime(void) const; 
            void SetMaxTime(float value);

            float GetVerticalForce(void) const; 
            void SetVerticalForce(float value);

            float GetHorizontalForce(void) const; 
            void SetHorizontalForce(float value);

            float GetOutwardForce(void) const; 
            void SetOutwardForce(float value);

            float GetGravityForce(void) const; 
            void SetGravityForce(float value);

            float GetRandomForce(void) const;
            void SetRandomForce(float value);

            float GetPulseSpeed(void) const; 
            void SetPulseSpeed(float value);

            float GetFadeAmount(void) const; 
            void SetFadeAmount(float value);

            float GetTransparencyThreshold(void) const; 
            void SetTransparencyThreshold(float value);

            float GetNormalOffset(void) const; 
            void SetNormalOffset(float value);

            float GetSpinScalar(void) const;
            void SetSpinScalar(float value);

            Vec2 GetTextureUV(void) const;
            void SetTextureUV(Vec2 value);

            void Update(float dt);

            void ResetFragmentation(void);

            float GetTimeSliderValue(void) const;
            void SetTimeSliderValue(float value);

            void StartFragmentation(void);
            void PauseFragmentation(void);

            bool GetPlayInReverse(void) const;
            void SetPlayInReverse(bool playReverse);

            Color GetColor(void) const;
            void SetColor(const Color &color);

        private:

            graphics::GfxAPI *m_graphics;

            resources::ResourceReference m_textureResource;

            void invalidateTexture(bool unload = true);

            void onTextureReload(void);

            bool m_isPlaying;

            bool m_playReverse;

        } Meta(Enable, WhiteListMethods, DisplayName("ModelFragmenter"))
        EditorMeta(
            RequiresComponents(typeof(ursine::ecs::Model3D))
        );
    }
}