namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            template<typename T>
            void ResourceWriter::Write(std::ostream &stream, const T &value)
            {
                WriteBytes(
                    stream,
                    reinterpret_cast<const char*>( std::addressof( value ) ), 
                    sizeof( T ) 
                );
            }

            template<typename T>
            ResourceWriter &ResourceWriter::Write(const T &value) 
            {
                WriteBytes(
                    m_stream,
                    reinterpret_cast<const char*>( std::addressof( value ) ), 
                    sizeof( T ) 
                );

                return *this;
            }
        }
    }
}