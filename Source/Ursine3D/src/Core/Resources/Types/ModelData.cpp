/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ModelData.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ModelData.h"
#include "ModelReader.h"
#include <Core/CoreSystem.h>
#include "GfxAPI.h"

namespace ursine
{
    namespace resources
    {
        ModelData::ModelData(graphics::ufmt_loader::ModelInfo modelInfo)
        {
            m_modelHandle = GetCoreSystem( graphics::GfxAPI )->ResourceMgr.CreateModel( modelInfo );
        }
        
        ModelData::~ModelData(void) 
        { 
            GetCoreSystem( graphics::GfxAPI )->ResourceMgr.DestroyModel( m_modelHandle );
        }
        
        graphics::GfxHND ModelData::GetModelHandle(void) const
        {
            return m_modelHandle;
        }

        void ModelData::Write(pipeline::ResourceWriter &output)
        {
            auto *modelInfo = GetCoreSystem( graphics::GfxAPI )->ResourceMgr.GetModelInfo( m_modelHandle );

            modelInfo->Write( output );
        }

        meta::Type ModelData::GetReaderType(void)
        {
            return typeof( ModelReader );
        }
    }
}
