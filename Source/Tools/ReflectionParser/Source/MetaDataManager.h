#pragma once

#include "Cursor.h"

class MetaDataManager
{
public:
    MetaDataManager(const Cursor &cursor);

    std::string GetProperty(const std::string &key) const;
    bool GetFlag(const std::string &key) const;

    void CompileTemplateData(TemplateData &data) const;
private:
    typedef std::pair<std::string, std::string> Property;

    std::unordered_map<std::string, std::string> m_properties;

    static const TemplateData::PartialType m_tmplInitializerListPartial;

    std::vector<Property> extractProperties(const Cursor &cursor) const;
};