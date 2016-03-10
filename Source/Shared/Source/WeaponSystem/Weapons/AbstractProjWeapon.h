#pragma once


#include "AbstractWeapon.h"
#include <Raycasting.h>

#define PROJ_EDITOR_FIELDS( )             \
    EditorField(                          \
        float ProjectileSpeed,            \
        GetProjSpeed,                     \
        SetProjSpeed                      \
    );                                    \
                                          \
    EditorResourceField(                  \
        ursine::resources::ArchetypeData, \
        archetypeToShoot,                 \
        GetArchetypeToShoot,              \
        SetArchetypeToShoot               \
    );                                    \
AbstractWeaponFields( )

struct AbstractProjWeapon : AbstractWeapon
{

    AbstractProjWeapon(void);

    float GetProjSpeed(void) const;
    void SetProjSpeed(const float speed);

    const ursine::resources::ResourceReference &GetArchetypeToShoot(void) const;
    void SetArchetypeToShoot(const ursine::resources::ResourceReference &archetype);

protected:

    // projectile speed
    float m_projSpeed;

    // Archetype weapon should fire
    ursine::resources::ResourceReference m_archetypeToShoot;
};
