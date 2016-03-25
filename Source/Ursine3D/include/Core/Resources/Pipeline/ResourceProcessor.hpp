namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            template<typename InputType>
            InputType *ResourceProcessor::expectInput(ResourceData::Handle input)
            {
                static const auto expectedType = typeof( InputType );

                UAssertCatchable( input && input->GetType( ) == expectedType,
                    "Expected type '%s' as input, got '%s'.",
                    expectedType.GetName( ).c_str( ),
                    input->GetType( ).GetName( ).c_str( )
                );

                return static_cast<InputType*>( input.get( ) );
            }
        }
    }
}