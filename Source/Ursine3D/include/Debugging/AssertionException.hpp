namespace ursine
{
    inline AssertionException::AssertionException(void)
        : m_file( "" )
        , m_function( "" )
        , m_line( 0 )
        , m_message( "" ) { }

    template<typename ... Args>
    AssertionException::AssertionException(URSINE_FFL_ARGS, URSINE_LOG_FORMATTED)
    {
        m_file = fs::path( file ).filename( ).string( );
        m_function = function;
        m_line = line;

        // extra space for '\0'
        auto size = snprintf( nullptr, 0, format.c_str( ), args ... ) + 1; 

        std::unique_ptr<char[]> buf( new char[ size ] ); 

        snprintf( buf.get( ), size, format.c_str( ), args... );

        // we don't want the '\0' inside
        m_message = std::string( buf.get( ), buf.get( ) + size - 1 ); 
    }

    inline const std::string &AssertionException::GetFile(void) const
    {
        return m_file;
    }

    inline const std::string &AssertionException::GetFunction(void) const
    {
        return m_function;
    }

    inline uint AssertionException::GetLine(void) const
    {
        return m_line;
    }

    inline const std::string &AssertionException::GetErrorMessage(void) const
    {
        return m_message;
    }
}