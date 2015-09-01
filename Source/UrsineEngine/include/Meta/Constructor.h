#pragma once

#include "MetaContainer.h"

#include "Invokable.h"

#include <functional>

namespace ursine
{
    class Type;

    class Constructor 
        : public MetaContainer
        , public Invokable
    {
    public:
        typedef std::function<Variant(ArgumentList&)> Invoker;

        Constructor(void);
        Constructor(Type classType, InvokableSignature signature, Invoker invoker);

        static const Constructor &Invalid(void);

        Type GetClassType(void) const;

        bool IsValid(void) const;

        Variant InvokeVariadic(ArgumentList &arguments) const;

        template<typename ...Args>
        Variant Invoke(Args &&...args) const;

    private:
        Type m_classType;

        Invoker m_invoker;
    };
}

#include "Impl/Constructor.hpp"