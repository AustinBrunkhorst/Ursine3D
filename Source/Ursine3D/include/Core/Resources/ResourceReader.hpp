namespace ursine
{
    namespace resources
    {
        template<typename T>
        ResourceReader &ResourceReader::operator>>(T &input) 
        {
            m_stream >> input;

            return *this;
        }
    }
}