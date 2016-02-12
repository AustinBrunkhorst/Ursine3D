namespace ursine
{
    namespace resources
    {
        template<typename T>
        ResourceReader &ResourceReader::operator>>(T &output) 
        {
            m_stream >> output;

            return *this;
        }
    }
}