#pragma once

#include "GlobalSetterBase.h"

namespace ursine
{
    namespace meta
    {
        template<typename GlobalType, bool IsMethod>
        class GlobalSetter : public GlobalSetterBase
        {
        public:
            void SetValue(const Variant &value) override;
        };
    }
}

#include "Impl/GlobalSetter.hpp"