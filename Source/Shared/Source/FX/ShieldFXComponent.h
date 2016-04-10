/*---------------------------------------------------------------------------
** Team Bear King
** 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ShieldFXComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <FragmentationComponent.h>

class ShieldFX : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    enum ShieldState : unsigned
    {
        SHIELD_STATE_STABLE = 0,
        SHIELD_STATE_DESTROYING,
        SHIELD_STATE_REBUILDING,

        SHIELD_STATE_COUNT
    };

    EditorButton(
        Stabilize,
        "Stabilize Shield"
    );

    EditorButton(
        Destroy,
        "Destroy Shield"
    );

    EditorButton(
        Rebuild,
        "Rebuild Shield"
    );

    Meta(Enable)
    ShieldFX(void);

    Meta(Enable)
    void OnInitialize(void) override;

    ShieldState GetShieldState(void) const;
    void SetShieldState(ShieldState state);

    void StabilizeShield(void);
    void DestroyShield(void);
    void RebuildShield(void);

private:
    ShieldState m_shieldState;

} 
Meta(Enable, WhiteListMethods, DisplayName("ShieldFX"))
EditorMeta(
    RequiresComponents(typeof(ursine::ecs::ModelFragmenter))
);
