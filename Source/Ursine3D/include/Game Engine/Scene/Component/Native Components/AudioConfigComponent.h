/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WorldConfig.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"

#include "AudioData.h"
#include "Array.h"

namespace ursine
{
    namespace ecs
    {
        struct VolumeControl
        {
            VolumeControl(void) { }

            std::string outputTypeName;
            std::string rtpcName;
        } Meta( Enable, EnableArrayType );

        class AudioConfig : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorResourceField(
                ursine::resources::AudioData,
                mainBank,
                GetMainBank,
                SetMainBank
            );

            ursine::Array<ursine::ecs::VolumeControl> volumeControls;

            Meta(Enable)
            AudioConfig(void);
            ~AudioConfig(void);

            const resources::ResourceReference &GetMainBank(void);
            void SetMainBank(const resources::ResourceReference &mainBank);

        private:
            resources::ResourceReference m_mainBank;

            void OnSceneReady(Scene *scene) override;

            void invalidateMainBank(void);

            void onVolumeChange(EVENT_HANDLER(World));

        } Meta(
            Enable, 
            WhiteListMethods, 
            DisplayName( "AudioConfig" )
        ) EditorMeta(
            DisableComponentRemoval
        );
    }
}