#pragma once

#include "MetaManager.h"

namespace Ursine
{
    class MetaContainer
    {
    public:
        MetaManager &GetMeta(void);

    private:
        MetaContainer &operator=(const MetaContainer &rhs) = delete;

        friend class ReflectionDatabase;
        friend struct TypeData;

        MetaManager m_meta;
    };
}