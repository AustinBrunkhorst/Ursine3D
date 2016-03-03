#include "UrsinePrecompiled.h"

#include "ModelReader.h"
#include "ModelData.h"

namespace ursine
{
    namespace resources
    {
        ModelReader::ModelReader(void) 
            : m_modelInfo( std::make_shared<graphics::ufmt_loader::ModelInfo>( ) )
        {

        }

        ResourceData::Handle ModelReader::Read(ResourceReader &input)
        {
            m_modelInfo->Read( input );

            return std::make_shared<ModelData>( m_modelInfo );
        }
    }
}