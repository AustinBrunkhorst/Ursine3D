/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ArchetypeData.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ArchetypeData.h"
#include "ArchetypeReader.h"

#include "EntitySerializer.h"

namespace ursine
{
    namespace resources
    {
        ArchetypeData::ArchetypeData(const Json &archetypeJson)
            : m_data( archetypeJson ) { }

        const Json &ArchetypeData::GetData(void)
        {
            return m_data;
        }

        ecs::EntityHandle ArchetypeData::Instantiate(ecs::World *world) const
        {
            ecs::EntitySerializer serializer;

            return serializer.DeserializeArchetype( world, m_data );
        }

        void ArchetypeData::Write(pipeline::ResourceWriter &output)
        {
            auto jsonString = m_data.dump( false );

            output.Write( jsonString );
        }

        meta::Type ArchetypeData::GetReaderType(void)
        {
            return typeof( ArchetypeReader );
        }
    }
}