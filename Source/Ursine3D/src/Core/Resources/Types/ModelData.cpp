#include "UrsinePrecompiled.h"

#include "ModelData.h"
#include "ModelReader.h"

namespace ursine
{
    namespace resources
    {
        ModelData::ModelData(graphics::ufmt_loader::ModelInfo *modelInfo)
        {
            // @@@TODO: give model info to graphics system, assign m_modelHandle
        }

        ModelData::~ModelData(void) { }

        void ModelData::Write(pipeline::ResourceWriter &output)
        {
            
        }

        meta::Type ModelData::GetReaderType(void)
        {
            return typeof( ModelReader );
        }
    }
}