/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ModelReader.cpp
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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