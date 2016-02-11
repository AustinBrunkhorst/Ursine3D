#include "UrsinePrecompiled.h"

#include "ResourceMetaData.h"

namespace ursine
{
    namespace
    {
        const auto kMetaDataVersion = "1.0";

        const auto kFieldMetaDataVersion = "v";
        const auto kFieldGUID = "guid";
        const auto kFieldOptions = "options";
    }

    const GUID &ResourceMetaData::GetGUID(void) const
    {
        return m_guid;
    }

    ResourceImportOptions *ResourceMetaData::GetImportOptions(void)
    {
        return m_importOptions;
    }

    void ResourceMetaData::OnSerialize(Json::object &output) const
    {
        output[ kFieldMetaDataVersion ] = kMetaDataVersion;

        output[ kFieldGUID ] = to_string( m_guid );

        output[ kFieldOptions ] = meta::Variant { m_importOptions, meta::variant_policy::WrapObject( ) }.SerializeJson( );
    }

    void ResourceMetaData::OnDeserialize(const Json &input)
    {
        
    }
}