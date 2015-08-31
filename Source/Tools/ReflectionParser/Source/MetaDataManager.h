#pragma once

#include "Cursor.h"

class MetaDataManager
{
public:
    MetaDataManager(const Cursor &cursor);

    std::string GetProperty(const std::string &key) const;
    bool GetFlag(const std::string &key) const;

private:
    std::unordered_map<std::string, std::string> m_properties;
};