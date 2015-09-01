#include "Precompiled.h"

#include "LanguageType.h"

LanguageType::LanguageType(const Cursor &cursor, const Namespace &currentNamespace)
    : m_metaData( cursor )
    , m_enabled( m_metaData.GetFlag( kMetaEnable ) )
    , m_ptrTypeEnabled( !m_metaData.GetFlag( kMetaDisablePtrType ) )
    , m_constPtrTypeEnabled( !m_metaData.GetFlag( kMetaDisableConstPtrType ) )
    , m_accessModifier( cursor.GetAccessModifier( ) )
{
    URSINE_TODO( "mess around with whitelist vs. blacklist" );
}

const MetaDataManager &LanguageType::GetMetaData(void) const
{
    return m_metaData;
}