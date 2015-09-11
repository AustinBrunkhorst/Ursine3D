#pragma once

class CursorType
{
public:
    CursorType(const CXType &handle);

    std::string GetDisplayName(void) const;

    int GetArgumentCount(void) const;
    CursorType GetArgument(unsigned index) const;

    CursorType GetCanonicalType(void) const;

    bool IsConst(void) const;

private:
    CXType m_handle;
};