#pragma once


#include "AbstractWeapon.h"
#include <Raycasting.h>

#define PROJ_EDITOR_FIELDS( )             \
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

    const ursine::resources::ResourceReference &GetArchetypeToShoot(void) const;
    void SetArchetypeToShoot(const ursine::resources::ResourceReference &archetype);

protected:

    // Archetype weapon should fire
    ursine::resources::ResourceReference m_archetypeToShoot;
};
