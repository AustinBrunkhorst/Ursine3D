/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GameAudioConfig.h
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

#include <GameManager.h>

struct VolumeControl
{
    VolumeControl(void) { }

    std::string outputTypeName;
    std::string rtpcName;
} Meta(Enable, EnableArrayType);

class GameAudioConfig : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorResourceField(
        ursine::resources::AudioData,
        mainBank,
        GetMainBank,
        SetMainBank
    );

    ursine::Array<VolumeControl> volumeControls;

    Meta(Enable)
    GameAudioConfig(ursine::GameManager *manager);

    Meta(Enable)
    GameAudioConfig(void);

    ~GameAudioConfig(void);

    const ursine::resources::ResourceReference &GetMainBank(void);
    void SetMainBank(const ursine::resources::ResourceReference &mainBank);

private:
    ursine::GameManager *m_manager;
    ursine::resources::ResourceReference m_mainBank;

    void invalidateMainBank(void);

    void onVolumeChange(EVENT_HANDLER(World));
} Meta(
    Enable, 
    WhiteListMethods
);