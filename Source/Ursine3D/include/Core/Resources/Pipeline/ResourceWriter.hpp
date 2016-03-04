namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            template<typename T>
            ResourceWriter &ResourceWriter::Write(const T &value) 
            {
                WriteBytes( 
                    reinterpret_cast<const char*>( std::addressof( value ) ), 
                    sizeof( T ) 
                );

                return *this;
            }
        }
    }
}