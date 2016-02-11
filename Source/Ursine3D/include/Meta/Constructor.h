/* ----------------------------------------------------------------------------
** Team Bear King
** © 201x DigiPen Institute of Technology, All Rights Reserved.
**
** Constructor.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "MetaContainer.h"

#include "Invokable.h"

#include "ConstructorInvoker.h"

namespace ursine
{
    namespace meta
    {
        class Type;

        class Constructor
            : public MetaContainer
            , public Invokable
        {
        public:
            Constructor(void);
            Constructor(const Constructor &rhs);
            Constructor(const Constructor &&rhs);
            
            Constructor(
                Type classType, 
                InvokableSignature signature, 
                ConstructorInvokerBase *invoker, 
                bool isDynamic
            );

            Constructor &operator=(const Constructor &&rhs);

            static const Constructor &Invalid(void);

            Type GetClassType(void) const;

            bool IsValid(void) const;
            bool IsDynamic(void) const;

            Variant InvokeVariadic(ArgumentList &arguments) const;

            template<typename ...Args>
            Variant Invoke(Args &&...args) const;

        private:
            bool m_isDynamic;

            Type m_classType;

            std::shared_ptr<ConstructorInvokerBase> m_invoker;
        };
    }
}

#include "Impl/Constructor.hpp"