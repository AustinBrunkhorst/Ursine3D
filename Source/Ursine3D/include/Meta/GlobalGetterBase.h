#pragma once

namespace ursine
{
    namespace meta
    {
        class GlobalGetterBase
        {
        public:
            virtual ~GlobalGetterBase(void) { }

            virtual Variant GetValue(void) = 0;
        };
    }
}