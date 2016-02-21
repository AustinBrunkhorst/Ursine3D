#include "ResourceData.h"

namespace ursine
{
    namespace resources
    {
        class JsonData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            JsonData(void) = default;

        private:
            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Enable);
    }
}