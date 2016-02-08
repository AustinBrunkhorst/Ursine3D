#pragma once

namespace ursine
{
    namespace meta
    {
        class FieldGetterBase
        {
        public:
            virtual ~FieldGetterBase(void) { }

            virtual Variant GetValue(const Variant &obj) = 0;
        };
    }
}