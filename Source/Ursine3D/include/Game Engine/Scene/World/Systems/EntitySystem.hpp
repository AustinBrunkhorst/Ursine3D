/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntitySystem.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace ursine
{
    namespace ecs
    {
        inline SystemTypeID EntitySystem::GetTypeID(void) const
        {
            return m_typeID;
        }

        inline SystemTypeMask EntitySystem::GetTypeMask(void) const
        {
            return 1ull << m_typeID;
        }
    }
}