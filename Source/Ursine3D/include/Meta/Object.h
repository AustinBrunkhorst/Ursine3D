/* ----------------------------------------------------------------------------
** Team Bear King
** © 201x DigiPen Institute of Technology, All Rights Reserved.
**
** Object.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "Json.h"

namespace ursine
{
    namespace meta
    {
        class Type;

        class Object
        {
        public:
            virtual ~Object(void) { }

            virtual Type GetType(void) const = 0;
            virtual Object *Clone(void) const = 0;

            virtual void OnSerialize(Json::object &output) const { }
            virtual void OnDeserialize(const Json &input) { }
        };
    }
}