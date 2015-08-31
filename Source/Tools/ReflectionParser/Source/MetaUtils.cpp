#include "Precompiled.h"

#include <Utils.h>

namespace utils
{
    void ToString(const CXString &str, std::string &output)
    {
        auto cstr = clang_getCString( str );

        output = cstr;

        clang_disposeString( str );
    }

    std::string GetQualifiedName(const Cursor &cursor, const Namespace &currentNamespace)
    {
        std::string name;

        Ursine::Utils::Join( currentNamespace, "::", name );

        if (!currentNamespace.empty( ))
            name += "::";

        name += cursor.GetSpelling( );

        return name;
    }
}