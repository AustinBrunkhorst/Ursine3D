#pragma once

#include "Type.h"

#include <string>
#include <vector>

namespace Ursine
{
    class Type;
    class Variant;
    class Argument;

    class EnumBase
    {
    public:
        EnumBase(const std::string &name, TypeID owner);

        virtual ~EnumBase(void) { }

        virtual Type GetType(void) const = 0;
        virtual Type GetUnderlyingType(void) const = 0;

        virtual std::vector<std::string> GetKeys(void) const = 0;
        virtual std::vector<Variant> GetValues(void) const = 0;

        virtual std::string GetKey(const Argument &value) const = 0;
        virtual Variant GetValue(const std::string &key) const = 0;

        Type GetParentType(void) const;

        const std::string &GetName(void) const;

    private:
        Type m_parentType;

        std::string m_name;
    };
}