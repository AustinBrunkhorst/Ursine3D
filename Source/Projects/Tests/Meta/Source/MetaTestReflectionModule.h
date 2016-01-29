#pragma once

#include <Meta/ReflectionModule.h>

namespace meta_generated
{
    class ModuleMetaTest : public ursine::meta::ReflectionModule
    {
    public:
        ModuleMetaTest(ursine::meta::ReflectionDatabase &db);
        ~ModuleMetaTest(void);
    };
}