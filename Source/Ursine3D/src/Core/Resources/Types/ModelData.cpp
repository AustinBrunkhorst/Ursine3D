#include "UrsinePrecompiled.h"

#include "ModelData.h"
#include "ModelReader.h"
#include <Core/CoreSystem.h>
#include "GfxAPI.h"
#include "ModelResource.h"

namespace ursine
{
    namespace resources
    {
        ModelData::ModelData(graphics::ufmt_loader::ModelInfo *modelInfo)
        {
            // @@@TODO: give model info to graphics system, assign m_modelHandle
            m_modelHandle = GetCoreSystem( graphics::GfxAPI )->ResourceMgr.CreateModel( modelInfo );
        }
        
        ModelData::~ModelData(void) { 
            GetCoreSystem( graphics::GfxAPI )->ResourceMgr.DestroyModel( m_modelHandle );
        }
        
        graphics::GfxHND ModelData::GetModelHandle(void) const
        {
            return m_modelHandle;
        }

        void ModelData::Write(pipeline::ResourceWriter &output)
        {            
            graphics::ufmt_loader::ModelInfo modelInfo = GetCoreSystem(graphics::GfxAPI)->ResourceMgr.GetModelInfo(m_modelHandle);

            modelInfo.SerializeOut(output);
        }

        meta::Type ModelData::GetReaderType(void)
        {
            return typeof( ModelReader );
        }
    }
}