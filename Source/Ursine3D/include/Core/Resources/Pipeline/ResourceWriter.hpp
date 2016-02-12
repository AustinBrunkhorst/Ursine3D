namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            template<typename T>
            ResourceWriter &ResourceWriter::operator<<(const T &value) 
            {
                m_stream << value;

                return *this;
            }
        }
    }
}