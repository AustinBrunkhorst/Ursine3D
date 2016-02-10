#pragma once

#include "InvokableConfig.h"

namespace ursine
{
    namespace meta
    {
        class MethodInvokerBase
        {
        public:
            virtual ~MethodInvokerBase(void) { }

            virtual Variant Invoke(Variant &obj, const ArgumentList &arguments) = 0;
        };
    }
}