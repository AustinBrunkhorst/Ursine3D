#pragma once

#include "DestructorInvokerBase.h"

namespace ursine
{
    namespace meta
    {
        template<typename ClassType>
        class DestructorInvoker : public DestructorInvokerBase
        {
        public:
            void Invoke(const Variant &obj) override;
        };
    }
}

#include "Impl/DestructorInvoker.hpp"