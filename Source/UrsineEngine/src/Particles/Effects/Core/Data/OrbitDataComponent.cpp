#include "UrsinePrecompiled.h"

#include "OrbitDataComponent.h"
#include "Entity.h"

namespace Ursine
{
    namespace ECS
    {
        OrbitData::OrbitData(void)
            : BaseComponent()
            , _radius(0.0f)
        {

        }

        OrbitData::OrbitData(float Radius)
            : BaseComponent()
            , _radius(Radius)
        {

        }

        OrbitData::~OrbitData(void)
        {

        }

        float OrbitData::GetRadius(void) const
        {
            return _radius;
        }

        void OrbitData::SetRadius(float val)
        {
            _radius = val;
        }
    }
}
