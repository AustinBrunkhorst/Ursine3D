#include "UrsinePrecompiled.h"

#include "ModelReader.h"
#include "ModelData.h"

namespace ursine
{
    namespace resources
    {
        ModelReader::ModelReader(void) 
        {

        }

        ResourceData::Handle ModelReader::Read(ResourceReader &input)
        {
            graphics::ufmt_loader::ModelInfo modelInfo;

            modelInfo.Read( input );

            return std::make_shared<ModelData>(modelInfo);
        }
    }
}