namespace ursine
{
    namespace resources
    {
        template<typename T>
        ResourceReader &ResourceReader::Read(T &input) 
        {
            m_stream->read( 
                reinterpret_cast<char*>( std::addressof( input ) ), 
                sizeof( T ) 
            );

            return *this;
        }
    }
}