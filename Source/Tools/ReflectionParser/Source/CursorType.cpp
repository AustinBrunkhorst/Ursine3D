#include "Precompiled.h"

#include "CursorType.h"

CursorType::CursorType(const CXType &handle)
    : m_handle( handle )
{

}

std::string CursorType::GetDisplayName(void) const
{
    std::string displayName;

    utils::ToString( clang_getTypeSpelling( m_handle ), displayName );

    return displayName;
}