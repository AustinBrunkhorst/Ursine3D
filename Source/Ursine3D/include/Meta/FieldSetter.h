#pragma once

#include "FieldSetterBase.h"

namespace ursine
{
    namespace meta
    {
        template<typename ClassType, typename FieldType, bool IsMethod>
        class FieldSetter : public FieldSetterBase
        {
        public:
            void SetValue(Variant &obj, const Argument &value) override;
        };
    }
}

#include "Impl/FieldSetter.hpp"