#pragma once

#include "Invokable.h"

#include "Variant.h"
#include "Argument.h"

#include <functional>

namespace Ursine
{
    class Function
        : public MetaContainer
        , public Invokable
    {
    public:
        typedef std::function<Variant(ArgumentList&)> Invoker;

        Function(void);

        template<typename ReturnType, typename ...ArgTypes>
        Function(const std::string &name, ReturnType(*type)(ArgTypes...), Invoker invoker, Type parentType = Type::Invalid);

        static const Function &Invalid(void);

        Type GetParentType(void) const;

        bool IsValid(void) const;

        Variant InvokeVariadic(ArgumentList &arguments) const;

        template<typename ...Args>
        Variant Invoke(Args &&...args) const;

    private:
        Type m_parentType;

        Invoker m_invoker;
    };
}

#include "Impl/Function.hpp"