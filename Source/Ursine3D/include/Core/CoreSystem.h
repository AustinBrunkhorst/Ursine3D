/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CoreSystem.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Meta.h>
#include <Application.h>

#define CORE_SYSTEM               \
    public:                       \
        Meta(Enable)              \
        static unsigned SystemID; \
    private:                      \

#define CORE_SYSTEM_DEFINITION(type) \
    unsigned type::SystemID = -1;    \

#define GetCoreSystem(name) ursine::Application::Instance->CoreSystem<name>( )

namespace ursine
{
    namespace core
    {
        class CoreSystem
        {
        public:
            virtual ~CoreSystem(void) { }

            // called after all systems have been constructed
            Meta(Disable)
            virtual void OnInitialize(void) { }

            // called before all systems are removed
            Meta(Disable)
            virtual void OnRemove(void) { }
        } Meta(Enable);
    }
}