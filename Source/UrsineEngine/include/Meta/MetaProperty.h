#pragma once

#include "Object.h"

namespace ursine 
{ 
    namespace meta
    {
        class MetaProperty : public Object
        {
        public:
            virtual Type GetType(void) const;
        };

        template<typename PropertyType, typename ...Args>
        MetaProperty *MetaPropertyInitializer(Args&&... args);
    } 
}

#include "Impl/MetaProperty.hpp"