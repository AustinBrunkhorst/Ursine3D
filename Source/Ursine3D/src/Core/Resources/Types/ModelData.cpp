#include "UrsinePrecompiled.h"

#include "ModelData.h"
#include "ModelReader.h"

namespace ursine
{
    namespace resources
    {
        ModelData::ModelData(void) { }

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