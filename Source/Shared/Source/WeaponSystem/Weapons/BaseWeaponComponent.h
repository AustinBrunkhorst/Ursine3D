/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BaseWeaponComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#pragma once

#include "AbstractWeapon.h"
#include <Component.h>

struct BaseWeapon : ursine::ecs::Component, AbstractWeapon
{
    NATIVE_COMPONENT

public:

    EditorField(
        std::string ArchetypeToShoot,
        GetArchetypeToShoot,
        SetArchetypeToShoot
    );

    AbstractWeaponFields( )

    BaseWeapon(void);
    ~BaseWeapon(void);

    void OnInitialize(void) override;

    const std::string& GetArchetypeToShoot(void) const;
    void SetArchetypeToShoot(const std::string &archetype);

    void RemoveMySelf(void) override;


    // Archetype weapon should fire
    std::string m_archetypeToShoot;

} Meta(Enable , DisplayName("ProjectileWeapon"));
