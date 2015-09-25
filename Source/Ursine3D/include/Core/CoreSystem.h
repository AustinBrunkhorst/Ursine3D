#pragma once

#include "Meta.h"

#define CORE_SYSTEM               \
    public:                       \
        Meta(Enable)              \
        static unsigned SystemID; \
    private:                      \

#define CORE_SYSTEM_DEFINITION(type) \
    unsigned type::SystemID = -1;    \

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