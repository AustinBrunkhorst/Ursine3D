#pragma once

#include "Cursor.h"
#include "Namespace.h"

#include "MetaDataManager.h"

namespace language_types
{
    class LanguageType
    {
    public:
        LanguageType(const Cursor &cursor, const Namespace &currentNamespace);

        const MetaDataManager &GetMetaData(void) const;

    protected:
        MetaDataManager m_metaData;

        // determines if the type is enabled in reflection database generation
        bool m_enabled;

        // determines if the pointer type to this type will be generated
        // in the reflection database
        bool m_ptrTypeEnabled;

        // determines if the constant pointer type to this type will be
        // generated in the reflection database
        bool m_constPtrTypeEnabled;

    private:
    };
}