namespace ursine
{
    namespace resources
    {
        template<typename ResourceType>
        ResourceType *ResourceReference::Load(bool ignoreCache /*= false */) const
        {
            // not valid, same behavior as unresolved asset
            if (!IsValid( ))
                return nullptr;

            static const auto expectedType = typeof( ResourceType );

            auto data = m_manager->LoadReference( *this, ignoreCache );

            // if it's null, simply also return null as well - behavior is defined
            // by the user when the resource can't be loaded
            if (!data)
                return nullptr;

            auto dataType = data->GetType( );

            UAssert( dataType == expectedType,
                "Invalid resource type dereference.\nExpected '%s' got '%s'",
                expectedType.GetName( ).c_str( ),
                dataType.GetName( ).c_str( )
            );

            return static_cast<ResourceType*>( data.get( ) );
        }
    }
}
