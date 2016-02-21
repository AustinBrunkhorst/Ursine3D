#include "UrsinePrecompiled.h"

#include "WorldReader.h"
#include "WorldData.h"

namespace ursine
{
    namespace resources
    {
        WorldReader::WorldReader(void) { }

        ResourceData::Handle WorldReader::Read(ResourceReader &input)
        {
            // TODO:
            return std::make_shared<WorldData>( );
        }
    }
}