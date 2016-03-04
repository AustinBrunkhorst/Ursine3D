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

// Constructs a variant that wraps an object
#define ObjectVariant(object) ursine::meta::Variant { object, ursine::meta::variant_policy::WrapObject( ) }

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