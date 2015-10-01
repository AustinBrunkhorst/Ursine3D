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
    
}

const MetaDataManager &LanguageType::GetMetaData(void) const
{
    return m_metaData;
}