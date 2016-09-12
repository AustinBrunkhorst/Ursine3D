/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** WorldData.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "WorldData.h"
#include "WorldReader.h"

#include "WorldSerializer.h"

namespace ursine
{
    namespace resources
    {
        WorldData::WorldData(const Json &worldJson)
        {
            try
            {
                m_data = ecs::World::Handle( ecs::WorldSerializer::Deserialize( worldJson ) );
            }
            catch (ecs::SerializationException &e)
            {
                UAssertCatchable( false, 
                    "Unable to deserialize world.\nerror: %s",
                    e.GetError( ).c_str( ) 
                );
            }
        }

        ecs::World::Handle WorldData::GetData(void)
        {
            return m_data;
        }

        void WorldData::Write(pipeline::ResourceWriter &output)
        {
            auto json = ecs::WorldSerializer::Serialize( m_data.get( ) );

            auto jsonString = json.dump( false );

            output.Write( jsonString );
        }

        meta::Type WorldData::GetReaderType(void)
        {
            return typeof( WorldReader );
        }
    }
}