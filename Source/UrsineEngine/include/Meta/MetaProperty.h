#pragma once

#include "Object.h"

namespace ursine 
{ 
    namespace meta
    {
        class MetaProperty : public Object
        {

        };

        template<typename PropertyType, typename ...Args>
        MetaProperty *MetaPropertyInitializer(Args&&... args);
    } 
}

#include "Impl/MetaProperty.hpp"