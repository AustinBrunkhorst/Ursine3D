/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Command.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <Entity/Entity.h>

class Command
{
public:
    virtual ~Command(void);
    virtual void Execute(ursine::ecs::Entity *receiver);
    virtual void StopExecute(ursine::ecs::Entity *receiver);

    unsigned GetWeight(void) const;

protected:
    int m_weight;
};