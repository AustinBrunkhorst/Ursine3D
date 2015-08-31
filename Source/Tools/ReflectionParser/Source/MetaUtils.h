#pragma once

#include "Cursor.h"
#include "Namespace.h"

namespace utils
{
    void ToString(const CXString &str, std::string &output);
    std::string GetQualifiedName(const Cursor &cursor, const Namespace &currentNamespace);
}