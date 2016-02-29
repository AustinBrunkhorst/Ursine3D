/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DamageEvent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <WorldEvent.h>
#include <EventArgs.h>
#include <SVec3.h>

enum DamageEvent
{
    WORLD_DAMAGE_EVENT = ursine::ecs::WORLD_EVENT_USER
};

class DamageEventArgs : public ursine::EventArgs
{
public:
    DamageEventArgs(float damage, bool crit,
                    const ursine::SVec3 &hitPosition,
                    ursine::ecs::Entity *entityHit)
        : damage( damage )
        , crit( crit )
        , hitPosition(hitPosition)
        , entityHit( entityHit )
        { }

    float damage;
    bool crit;
    ursine::SVec3 hitPosition;
    ursine::ecs::Entity *entityHit;

};
