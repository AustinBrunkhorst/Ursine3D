#pragma once

#include "InvokableConfig.h"

namespace ursine
{
    namespace meta
    {
        class ConstructorInvokerBase
        {
        public:
            virtual ~ConstructorInvokerBase(void) { }

            virtual Variant Invoke(const ArgumentList &arguments) = 0;
        };
    }
}