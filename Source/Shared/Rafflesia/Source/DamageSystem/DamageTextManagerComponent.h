/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DamageTextManagerComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <ArchetypeData.h>

#include "DamageTextComponent.h"

class DamageTextManager : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorResourceField(
        ursine::resources::ArchetypeData,
        damageSpriteText,
        GetDamageSpriteText,
        SetDamageSpriteText
    );

    EditorResourceField(
        ursine::resources::ArchetypeData,
        critSpriteText,
        GetCritSpriteText,
        SetCritSpriteText
    );

    EditorResourceField(
        ursine::resources::ArchetypeData,
        invulnerableSpriteText,
        GetInvulnerableSpriteText,
        SetInvulnerableSpriteText
    );

    DamageTextManager(void);
    ~DamageTextManager(void);

    void OnSceneReady(ursine::Scene *scene) override;

    const ursine::resources::ResourceReference &GetDamageSpriteText(void) const;
    void SetDamageSpriteText(const ursine::resources::ResourceReference &archetype);

    const ursine::resources::ResourceReference &GetCritSpriteText(void) const;
    void SetCritSpriteText(const ursine::resources::ResourceReference &archetype);

    const ursine::resources::ResourceReference &GetInvulnerableSpriteText(void) const;
    void SetInvulnerableSpriteText(const ursine::resources::ResourceReference &archetype);

private:

    ursine::resources::ResourceReference m_damageText;
    ursine::resources::ResourceReference m_critText;
    ursine::resources::ResourceReference m_invulnerableText;

    void onDamageText(EVENT_HANDLER(ursine::ecs::World));

} Meta(Enable);
