#include "ResourceData.h"

namespace ursine
{
    namespace resources
    {
        class ModelData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            ModelData(void);
            ~ModelData(void);

        private:
            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}