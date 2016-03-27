#include "UrsinePrecompiled.h"

#include "BinaryData.h"

namespace ursine
{
    BinaryData::BinaryData(void)
        : m_data( nullptr ) 
        , m_size( 0 ) { }

    BinaryData::BinaryData(BinaryData &&rhs) 
        : m_data( std::move( rhs.m_data ) ) 
        , m_size( rhs.m_size )
    {
        rhs.m_size = 0;
    }

    BinaryData::BinaryData(void *data, size_t size) 
        : m_data( new uint8[ size ] )
        , m_size( size )
    {
        memcpy( m_data.get( ), data, size );
    }

    BinaryData &BinaryData::operator=(BinaryData &&rhs) 
    {
        m_data = std::move( rhs.m_data );
        m_size = rhs.m_size;

        rhs.m_size = 0;

        return *this;
    }

    BinaryData BinaryData::Copy(const BinaryData &rhs)
    {
        return BinaryData( rhs.m_data.get( ), rhs.m_size );
    }

    void BinaryData::Load(BinaryData &input, void *data, size_t size)
    {
        input.m_data = Data( reinterpret_cast<byte*>( data ) );
        input.m_size = size;
    }

    const void *BinaryData::GetData(void) const
    {
        return m_data.get( );
    }

    size_t BinaryData::GetSize(void) const
    {
        return m_size;
    }

    void BinaryData::CopyTo(void *destination) const
    {
        std::memcpy( destination, m_data.get( ), m_size );
    }
}
