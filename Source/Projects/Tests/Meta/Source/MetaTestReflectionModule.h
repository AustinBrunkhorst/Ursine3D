/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** MetaTestReflectionModule.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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