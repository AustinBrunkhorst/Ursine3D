#include "UrsinePrecompiled.h"

#include "ModelReader.h"
#include "ModelData.h"


namespace ursine
{
    namespace resources
    {
        ModelReader::ModelReader(void) { }

        ResourceData::Handle ModelReader::Read(ResourceReader &input)
        {
            m_modelInfo->SerializeIn(input);

            return std::make_shared<ModelData>( m_modelInfo );
        }
    }
}