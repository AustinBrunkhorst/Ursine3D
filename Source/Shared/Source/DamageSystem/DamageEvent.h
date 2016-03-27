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

namespace game
{
    enum DamageEvent
    {
        /////////////////////////
        ////  damage events  ////
        /////////////////////////
        DAMAGE_TEXT_EVENT = ursine::ecs::WORLD_EVENT_USER
    };

    class DamageEventArgs : public ursine::EventArgs
    {
    public:
        DamageEventArgs(const ursine::SVec3 &hitPosition,
                        ursine::ecs::EntityHandle &entityHit,
                        float damage, bool crit, bool invulnerable)
            : hitPosition( hitPosition )
            , entityHit( entityHit )
            , damage( damage )
            , crit( crit )
            , invulnerable( invulnerable ) { }

        ursine::SVec3 hitPosition;

        ursine::ecs::EntityHandle entityHit;

        float damage;

        bool crit;
        bool invulnerable;
    };
}
