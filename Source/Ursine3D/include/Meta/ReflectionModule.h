/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ReflectionModule.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ReflectionDatabase.h"

#define CONSTRUCT_MODULE(module) meta_generated::Module##module _##module( db );

#define DECLARE_REFLECTION_MODULE(module)                            \
    namespace meta_generated                                         \
    {                                                                \
        class Module##module : public ursine::meta::ReflectionModule \
        {                                                            \
        public:                                                      \
             Module##module(ursine::meta::ReflectionDatabase &db);   \
            ~Module##module(void);                                   \
        };                                                           \
    }                                                                \

namespace ursine
{
    namespace meta
    {
        class ReflectionModule
        {
        public:
            ReflectionModule(ReflectionDatabase &db);

        protected:
            ReflectionDatabase &db;
        };
    }
}