namespace ursine
{
    template<typename DataType>
    Json JsonSerializer::Serialize(DataType &instance)
    {
        static_assert( std::is_enum<DataType>::value ||
            std::is_integral<DataType>::value,
            "This data type doesn't have a JSON serializer!" );

        return Json( static_cast<int>( instance ) );
    }

    template<typename DataType>
    void JsonSerializer::Deserialize(const Json &data, DataType &out)
    {
        static_assert( std::is_enum<DataType>::value ||
            std::is_integral<DataType>::value,
            "This data type doesn't have a JSON deserializer!" );

        out = static_cast<DataType>( data.int_value( ) );
    }

    template<typename DataType, typename DefaultType>
    void JsonSerializer::Deserialize(const Json &data, DataType &out, const DefaultType &default_value)
    {
        if (data.is_null( ))
            out = default_value;
        else
            Deserialize( data, out );
    }
}
