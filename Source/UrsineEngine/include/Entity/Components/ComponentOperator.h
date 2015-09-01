/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ComponentOperator.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Component.h"

namespace ursine
{
    namespace ecs
    {
        struct ComponentOperator final
        {
            typedef Component *(*Creator)(void);
            typedef Component *(*Copier)(const Component *);

            // Creates an instance of a component
            Creator Create;

            // Copies an instance of a component
            Copier Copy;
        };
    }
}