/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityProcessor.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace Ursine
{
    namespace ECS
    {
        // forward declaration
        class Entity;

        class EntityProcessor
        {
        protected:
            // Called before this processor actually processes
            virtual void Begin(void) { }

            // Called on each interesting entity when processing
            virtual void Process(Entity *entity) = 0;

            // Called after processing for this frame has complete
            virtual void End(void) { }

        public:
            virtual ~EntityProcessor(void) { }
        };
    }
}
