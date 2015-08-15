/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** OrbitDataComponent.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef ORBIT_DATA_COMPONENT_H
#define ORBIT_DATA_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

namespace Ursine
{
    namespace ECS
    {
        class OrbitData : public Component
        {
            SerializerAllow(OrbitData, _radius);
            float _radius;

        public:
            OrbitData(void);
            OrbitData(float radius);

            ~OrbitData(void);

            float GetRadius(void) const;
            void SetRadius(float val);
        };
    }
}

////////////////////////////////////////////////////////////////////////////////
#endif // !ORBIT_DATA_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_COMPONENTS
#ifndef ORBIT_DATA_COMPONENT_REGISTERED
#define ORBIT_DATA_COMPONENT_REGISTERED

namespace Ursine
{
    namespace ECS
    {
        RegisterComponent(OrbitData);
        RegisterComponentData(OrbitData, _radius, "radius");
    }
}

#endif // !ORBIT_DATA_COMPONENT_REGISTERED
#endif // !REGISTER_COMPONENTS