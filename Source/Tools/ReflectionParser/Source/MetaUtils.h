#pragma once

#include "Cursor.h"
#include "Namespace.h"

#include "Mustache.h"

namespace utils
{
    void ToString(const CXString &str, std::string &output);

    Mustache::Data<std::string>::Type TemplateBool(bool value);

    std::string GetQualifiedName(const std::string &displayName, const Namespace &currentNamespace);
    std::string GetQualifiedName(const Cursor &cursor, const Namespace &currentNamespace);

    std::shared_ptr<std::string> LoadText(const std::string &filename);
    void WriteText(const std::string &filename, const std::string &text);

    void FatalError(const std::string &error);
}