#pragma once

namespace ursine
{
    namespace meta
    {
        class FieldSetterBase
        {
        public:
            virtual ~FieldSetterBase(void) { }

            virtual void SetValue(Variant &obj, const Argument &value) = 0;
        };
    }
}