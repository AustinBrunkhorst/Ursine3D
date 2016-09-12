/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TransformComponent.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace ursine
{
    namespace ecs
    {
        template <class ComponentType>
        ComponentType* Transform::GetComponentInChildren(void) const
        {
            return GetOwner( )->GetComponentInChildren<ComponentType>( );
        }

        template <class ComponentType>
        ComponentType* Transform::GetComponentInParents(void) const
        {
            return GetOwner( )->GetComponentInParents<ComponentType>( );
        }

        template <class ComponentType>
        std::vector<ComponentType*> Transform::GetComponentsInChildren(void) const
        {
            return GetOwner( )->GetComponentsInChildren<ComponentType>( );
        }

        template <class ComponentType>
        std::vector<ComponentType*> Transform::GetComponentsInParents(void) const
        {
            return GetOwner( )->GetComponentsInParents<ComponentType>( );
        }
    }
}
