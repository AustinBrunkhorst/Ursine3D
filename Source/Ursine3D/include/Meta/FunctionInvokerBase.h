#pragma once

#include "InvokableConfig.h"

namespace ursine
{
    namespace meta
    {
        class FunctionInvokerBase
        {
        public:
            virtual ~FunctionInvokerBase(void) { }

            virtual Variant Invoke(const ArgumentList &arguments) = 0;
        };
    }
}