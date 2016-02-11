#pragma once

#include "InvokableConfig.h"

namespace ursine
{
    namespace meta
    {
        class DestructorInvokerBase
        {
        public:
            virtual ~DestructorInvokerBase(void) { }

            virtual void Invoke(const Variant &obj) = 0;
        };
    }
}