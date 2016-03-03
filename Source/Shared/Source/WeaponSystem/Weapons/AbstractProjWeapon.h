#pragma once


#include "AbstractWeapon.h"
#include <Raycasting.h>

#define PROJ_EDITOR_FIELDS( )            \
    EditorField(                         \
        float ProjectileSpeed,           \
        GetProjSpeed,                    \
        SetProjSpeed                     \
    );                                   \
                                         \
    EditorField(                         \
        std::string ArchetypeToShoot,    \
        GetArchetypeToShoot,             \
        SetArchetypeToShoot              \
    );                                   \
                                         \
AbstractWeaponFields( )

struct AbstractProjWeapon : AbstractWeapon
{

    AbstractProjWeapon(void);

    float GetProjSpeed(void) const;
    void SetProjSpeed(const float speed);

    const std::string& GetArchetypeToShoot(void) const;
    void SetArchetypeToShoot(const std::string &archetype);

protected:

    // projectile speed
    float m_projSpeed;

    // Archetype weapon should fire
    std::string m_archetypeToShoot;
};
