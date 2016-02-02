#pragma once

#include "ResourceImportOptions.h"

namespace ursine
{
    class ResourceMetaData : public meta::Object
    {
    public:
        const GUID &GetGUID(void) const;

    private:
        void OnSerialize(Json::object &output) const override;
        void OnDeserialize(const Json &input) override;

        ResourceImportOptions *m_importOptions;
        GUID m_guid;
    } Meta(Enable);
}