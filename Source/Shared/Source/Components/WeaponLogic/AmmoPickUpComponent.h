/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AmmoPickUpComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class AmmoPickup : public ursine::ecs::Component
{
    NATIVE_COMPONENT ;

public:

    AmmoPickup(void);
    ~AmmoPickup(void);

    void OnInitialize(void) override;

    EditorField(
        int AmmoCount,
        GetAmmoCount,
        SetAmmoCount
        );
    int GetAmmoCount(void) const;
    void SetAmmoCount(const int count);
    int m_count;
    
} Meta(Enable, DisplayName("AmmoPickup"));
