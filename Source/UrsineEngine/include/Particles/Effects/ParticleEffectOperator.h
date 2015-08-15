/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEffectOperator.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace Ursine
{
    namespace ECS
    {
        class ParticleEmitter;
    }

    class ParticleEffect;

    struct ParticleEffectOperator final
    {
        typedef ParticleEffect *(*Creator)(void);
        typedef ParticleEffect *(*Copier)(const ParticleEffect *);

        ParticleEffectOperator(void) = default;

        // Copies an instance of an effect
        Creator Create;
        
        // Copies an instance of an effect
        Copier Copy;
    };
}
