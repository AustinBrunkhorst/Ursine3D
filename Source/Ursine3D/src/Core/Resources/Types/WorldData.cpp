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
                UAssertCatchable( "Unable to deserialize world.\nerror: %s",
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