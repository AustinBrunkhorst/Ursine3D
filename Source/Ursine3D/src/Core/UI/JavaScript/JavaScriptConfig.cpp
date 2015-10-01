#include "UrsinePrecompiled.h"

#include "JavaScriptConfig.h"

namespace ursine
{
    namespace
    {
        void serialize(const CefRefPtr<CefV8Value> &input, Json &output);
        void deserialize(const Json &input, CefRefPtr<CefV8Value> &output);
    }

    template<>
    Json JsonSerializer::Serialize(const CefRefPtr<CefV8Value> &instance)
    {
        Json output;

        serialize( instance, output );

        return output;
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, CefRefPtr<CefV8Value> &out)
    {
        deserialize( data, out );
    }

    namespace
    {
        void serialize(const CefRefPtr<CefV8Value> &input, Json &output)
        {
            auto value = input.get( );

            if (value->IsBool( ))
            {
                output = value->GetBoolValue( );
            }
            else if (value->IsDouble( ))
            {
                output = value->GetDoubleValue( );
            }
            else if (value->IsInt( ))
            {
                output = value->GetIntValue( );
            }
            else if (value->IsUInt( ))
            {
                output = static_cast<int>( value->GetUIntValue( ) );
            }
            else if (value->IsString( ))
            {
                output = value->GetStringValue( ).ToString( );
            }
            else if (value->IsDate( ))
            {
                output = value->GetDateValue( ).GetDoubleT( );
            }
            else if (value->IsArray( ))
            {
                Json::array output_array;

                int length = value->GetArrayLength( );

                output_array.resize( length );

                for (int i = 0; i < length; ++i)
                    serialize( value->GetValue( i ), output_array[ i ] );

                output = output_array;
            }
            else if (value->IsObject( ))
            {
                Json::object output_obj;

                std::vector<CefString> keys;

                UAssert( value->GetKeys( keys ),
                    "Unable to get keys for JavaScript object." );

                for (auto &key : keys)
                {
                    auto item = output_obj.emplace( key, Json::object( ) );

                    UAssert( item.second, 
                        "Unable to add object at key '%s'",
                        key.ToString( ).c_str( ) );

                    serialize( value->GetValue( key ), item.first->second );
                }

                output = output_obj;
            }
            else
            {
                output = nullptr;
            }
        }

        void deserialize(const Json &input, CefRefPtr<CefV8Value> &output)
        {
            switch (input.type( ))
            {
            case Json::BOOL:
            {
                output = CefV8Value::CreateBool( input.bool_value( ) );
            }
            break;
            case Json::NUMBER:
            {
                output = CefV8Value::CreateDouble( input.number_value( ) );
            }
            break;
            case Json::STRING:
            {
                output = CefV8Value::CreateString( input.string_value( ) );
            }
            break;
            case Json::ARRAY:
            {
                auto &items = input.array_items( );

                auto size = static_cast<int>( items.size( ) );

                auto array = CefV8Value::CreateArray( size );

                for (int i = 0; i < size; ++i)
                {
                    CefRefPtr<CefV8Value> value;

                    deserialize( items[ i ], value );

                    array->SetValue( i, value );
                }

                output = array;
            }
            break;
            case Json::OBJECT:
            {
                auto &items = input.object_items( );

                auto object = CefV8Value::CreateObject( nullptr );

                for (auto &item : items)
                {
                    CefRefPtr<CefV8Value> value;

                    deserialize( item.second, value );

                    object->SetValue( 
                        item.first,
                        value,
                        V8_PROPERTY_ATTRIBUTE_NONE 
                    );
                }

                output = object;
            }
            break;
            default:
            {
                output = CefV8Value::CreateNull( );
            }
            }
        }
    }
}
