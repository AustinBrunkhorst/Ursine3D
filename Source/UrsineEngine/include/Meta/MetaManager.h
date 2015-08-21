#pragma once

#include <string>
#include <unordered_map>

namespace Ursine
{
    class MetaManager
    {
    public:
        struct Property
        {
            std::string key;
            std::string value;

            Property(void);
            Property(const std::string &key, const std::string &value = std::string( ));

            int ToInt(void) const;
            bool ToBool(void) const;
            float ToFloat(void) const;
            double ToDouble(void) const;
        };

        typedef std::initializer_list<Property> Initializer;

        MetaManager(void);
        MetaManager(const std::initializer_list<Property> &properties);

        // Gets a property with the given key. nullptr is returned 
        // if the key doesn't exist
        const Property *GetProperty(const std::string &key) const;

        // Determines if the given key exists
        bool GetFlag(const std::string &key) const;

        // Sets the given property
        void SetProperty(const Property &prop);

    private:
        std::unordered_map<std::string, Property> m_properties;
    };
}