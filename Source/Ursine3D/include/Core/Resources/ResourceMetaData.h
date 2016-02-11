#pragma once

#include "ResourceImportOptions.h"
#include "GUID.h"

namespace ursine
{
    class ResourceMetaData : public meta::Object
    {
        META_OBJECT;
    public:
        const GUID &GetGUID(void) const;
        ResourceImportOptions *GetImportOptions(void);

    private:
        void OnSerialize(Json::object &output) const override;
        void OnDeserialize(const Json &input) override;

        ResourceImportOptions *m_importOptions;
        GUID m_guid;
    } Meta(Enable);
}