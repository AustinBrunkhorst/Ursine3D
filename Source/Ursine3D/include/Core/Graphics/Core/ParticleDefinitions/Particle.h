/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** Particle.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "SVec3.h"
#include "Color.h"

namespace ursine
{
    namespace graphics
    {
        // information that will be mapped back to the gpu
        struct Particle_GPU
        {
            // alignment is really, really important
            // boundry ////////////
            float position[ 3 ];
            float scaleX;

            // boundry ////////////
            float rotation[ 3 ];
            float scaleY;

            // boundry ////////////
            float color[ 4 ];
        };

        // information that stays on the cpu, used for updating
        struct Particle_CPU
        {
            // in what direction is this particle going?
            SVec3 velocity;

            // how long does this particle have to live?
            float lifeTime;

            // how much life did this particle start with?
            float totalLifetime;

            // in what direction is this particle accelerating?
            SVec3 acceleration;

            float roll;
        };
    }
}