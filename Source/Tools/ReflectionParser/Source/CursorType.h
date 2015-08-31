#pragma once

class CursorType
{
public:
    CursorType(const CXType &handle);

    std::string GetDisplayName(void) const;

private:
    CXType m_handle;
};