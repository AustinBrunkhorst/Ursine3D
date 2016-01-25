/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LanguageType.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "LanguageType.h"

LanguageType::LanguageType(
	const Cursor &cursor, 
    const Namespace &currentNamespace
)
    : m_metaData( cursor )
    , m_enabled( m_metaData.GetFlag( native_property::Enable ) )
    , m_ptrTypeEnabled( !m_metaData.GetFlag( native_property::DisablePtrType ) )
    , m_constPtrTypeEnabled( !m_metaData.GetFlag( native_property::DisableConstPtrType ) )
    , m_accessModifier( cursor.GetAccessModifier( ) )
{
    std::cout << "display name: " << cursor.GetDisplayName( ) << std::endl;
    std::cout << " ------- usr: " << cursor.GetUSR() << std::endl;
}

const MetaDataManager &LanguageType::GetMetaData(void) const
{
    return m_metaData;
}