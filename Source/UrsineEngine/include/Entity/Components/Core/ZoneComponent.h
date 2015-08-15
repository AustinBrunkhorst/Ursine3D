/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ZoneComponent.h
** - Logic Zone Component
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** 
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef ZONE_COMPONENT_H
#define ZONE_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#include "Component.h"
#include "AABB.h"

namespace Ursine
{
    enum ZoneType
    {
        ZONE_ACCELERATION,
        ZONE_COUNT
    };

    namespace ECS
    {
        class Zone : public Component
        {
            friend class PhysicsDebugDrawer;
        public:
            Zone();
            ~Zone();
            Zone(const Zone &other);
            const Zone &operator=(const Zone &rhs);

            void OnInitialize(void) override;

            ZoneType type;

            void SetWidth(float width);
            void SetHeight(float height);
            
            float GetWidth(void);
            float GetHeight(void);

            void UpdateAABB(void);

            bool Contains(const AABB &aabb);

        private:

            SerializerAllow(Zone, width);
            SerializerAllow(Zone, height);
            float width, height;

            AABB _aabb;

            Transform *_transform;
        };
    }

    extern template
    Json JsonSerializer::Serialize(ZoneType &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, ZoneType &out);
}

////////////////////////////////////////////////////////////////////////////////
#endif // !ZONE_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_COMPONENTS
#ifndef ZONE_REGISTERED
#define ZONE_REGISTERED
namespace Ursine
{
    namespace ECS
    {
        RegisterComponent(Zone);
        RegisterComponentData(Zone, type);
        RegisterComponentData(Zone, width);
        RegisterComponentData(Zone, height);
    }
}
#endif
#endif
