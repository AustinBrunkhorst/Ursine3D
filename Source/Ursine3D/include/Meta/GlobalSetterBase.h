#pragma once

namespace ursine
{
    namespace meta
    {
        class GlobalSetterBase
        {
        public:
            virtual ~GlobalSetterBase(void) { }

            virtual void SetValue(const Variant &value) = 0;
        };
    }
}