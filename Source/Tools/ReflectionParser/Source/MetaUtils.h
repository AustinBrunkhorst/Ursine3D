#pragma once

#include "Cursor.h"
#include "Namespace.h"

namespace utils
{
    void ToString(const CXString &str, std::string &output);

    std::string GetQualifiedName(const std::string &displayName, const Namespace &currentNamespace);
    std::string GetQualifiedName(const Cursor &cursor, const Namespace &currentNamespace);

    std::shared_ptr<std::string> LoadText(const std::string &filename);
}