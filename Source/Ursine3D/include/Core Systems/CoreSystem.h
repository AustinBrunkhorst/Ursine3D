#pragma once

#include <Meta.h>

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
        } Meta(Enable);
    }
}