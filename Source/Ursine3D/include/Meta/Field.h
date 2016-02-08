/* ----------------------------------------------------------------------------
** Team Bear King
** © 201x DigiPen Institute of Technology, All Rights Reserved.
**
** Field.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "MetaContainer.h"

#include "Variant.h"

#include "FieldGetter.h"
#include "FieldSetter.h"

namespace ursine
{
    namespace meta
    {
        class Field : public MetaContainer
        {
        public:
            static bool SetValue(Variant &instance, const Variant &value, const Method &setter);

            Field(void);

            Field(
                const std::string &name, 
                Type type, 
                Type classType, 
                FieldGetterBase *getter, 
                FieldSetterBase *setter
            );

            ~Field(void);

            bool IsValid(void) const;
            bool IsReadOnly(void) const;

            Type GetType(void) const;
            Type GetClassType(void) const;

            const std::string &GetName(void) const;

            Variant GetValue(const Variant &instance) const;
            bool SetValue(Variant &instance, const Variant &value) const;
            
        private:
            Type m_type;
            Type m_classType;

            std::string m_name;

            FieldGetterBase *m_getter;
            FieldSetterBase *m_setter;
        };
    }
}