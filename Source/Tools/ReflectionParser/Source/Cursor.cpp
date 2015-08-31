#include "Precompiled.h"

#include "Cursor.h"

#include "MetaUtils.h"

Cursor::Cursor(const CXCursor &handle)
    : m_handle( handle ) { }

CXCursorKind Cursor::GetKind(void) const
{
    return m_handle.kind;
}

std::string Cursor::GetSpelling(void) const
{
    std::string spelling;

    utils::ToString( clang_getCursorSpelling( m_handle ), spelling );

    return spelling;
}

std::string Cursor::GetDisplayName(void) const
{
    std::string displayName;

    utils::ToString( clang_getCursorDisplayName( m_handle ), displayName );

    return displayName;
}

std::string Cursor::GetMangledName(void) const
{
    std::string mangled;

    utils::ToString( clang_Cursor_getMangling( m_handle ), mangled );

    return mangled;
}

bool Cursor::IsDefinition(void) const
{
    return clang_isCursorDefinition( m_handle ) ? true : false;
}

bool Cursor::IsStatic(void) const
{
    return clang_CXXMethod_isStatic( m_handle );
}

CX_CXXAccessSpecifier Cursor::GetAccessSpecifier(void) const
{
    return clang_getCXXAccessSpecifier( m_handle );
}

CX_StorageClass Cursor::GetStorageClass(void) const
{
    return clang_Cursor_getStorageClass( m_handle );
}

CursorType Cursor::GetType(void) const
{
    return clang_getCursorType( m_handle );
}

Cursor::List Cursor::GetChildren(void) const
{
    List children;

    auto visitor = [](CXCursor cursor, CXCursor parent, CXClientData data)
    {
        auto container = static_cast<List *>( data );

        container->emplace_back( cursor );

        if (cursor.kind == CXCursor_LastPreprocessing)
            return CXChildVisit_Break;

        return CXChildVisit_Continue;
    };

    clang_visitChildren( m_handle, visitor, &children );

    return children;
}

void Cursor::VisitChildren(Visitor visitor, void *data)
{
    clang_visitChildren( m_handle, visitor, data );
}