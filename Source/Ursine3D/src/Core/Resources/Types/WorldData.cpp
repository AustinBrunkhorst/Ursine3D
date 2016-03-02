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
            ecs::WorldSerializer serializer;

            try
            {
                m_data = ecs::World::Handle( serializer.Deserialize( worldJson ) );
            }
            catch (ecs::SerializationException &e)
            {
                UError( "Unable to deserialize world.\nerror: %s", 
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
            ecs::WorldSerializer serializer;

            auto json = serializer.Serialize( m_data.get( ) );

            auto jsonString = json.dump( false );

            output.WriteString( jsonString );
        }

        meta::Type WorldData::GetReaderType(void)
        {
            return typeof( WorldReader );
        }
    }
}