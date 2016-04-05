/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GraphicsSettingsComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "TextureData.h"
#include "ResourceData.h"

namespace ursine
{
    namespace ecs
    {
        class GraphicsSettings : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorMeta(InputRange(0.0f, 1.0f, 0.01f, "{{value.toPrecision( 2 )}}"))
            EditorField(
                float globalEmissive,
                GetGlobalEmissive,
                SetGlobalEmissive
            );

            EditorMeta(InputRange(1.0f, 1000.0f, 1.0f, "{{value.toPrecision( 2 )}}"))
            EditorField(
                float lightStepValue,
                GetLightStepValue,
                SetLightStepValue
            );

            EditorResourceField(
                ursine::resources::TextureData,
                lightMap,
                GetLightMapTexture,
                SetLightMapTexture
            );


            Meta(Enable)
            GraphicsSettings(void);
            ~GraphicsSettings(void);

            void OnInitialize(void) override;

            void OnSceneReady(Scene *scene) override;

            float GetGlobalEmissive(void) const;
            void SetGlobalEmissive(float value);

            float GetLightStepValue(void) const;
            void SetLightStepValue(float value);

            const resources::ResourceReference &GetLightMapTexture(void) const;
            void SetLightMapTexture(const resources::ResourceReference &texture);

        private:
            resources::ResourceReference m_lightMap;

            void invalidateLightmap(bool unload = true);

        } Meta(Enable, WhiteListMethods, DisplayName("GraphicsSettings"));
    }
}