/* ----------------------------------------------------------------------------
** Team Bear King
** © 201x DigiPen Institute of Technology, All Rights Reserved.
**
** Function.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "Invokable.h"

#include "Variant.h"
#include "Argument.h"

#include "FunctionInvoker.h"

#include <functional>

namespace ursine
{
    namespace meta
    {
        class Function
            : public MetaContainer
            , public Invokable
        {
        public:
            typedef std::function<Variant(ArgumentList&)> Invoker;

            Function(void);

            template<typename ReturnType, typename ...ArgTypes>
            Function(
                const std::string &name, 
                ReturnType (*function)(ArgTypes...),
                Type parentType = Type::Invalid
            );

            ~Function(void);

            static const Function &Invalid(void);

            Type GetParentType(void) const;

            bool IsValid(void) const;

            Variant InvokeVariadic(ArgumentList &arguments) const;

            template<typename ...Args>
            Variant Invoke(Args &&...args) const;

        private:
            Type m_parentType;

            FunctionInvokerBase *m_invoker;
        };
    }
}

#include "Impl/Function.hpp"