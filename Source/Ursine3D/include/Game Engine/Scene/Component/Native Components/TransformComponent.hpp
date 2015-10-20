/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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
		ComponentType* Transform::GetComponentInChildren(const Entity* entity) const
		{
			return GetOwner( )->GetComponentInChildren( entity );
		}

		template <class ComponentType>
		ComponentType* Transform::GetComponentInParent(const Entity* entity) const
		{
			return GetOwner( )->GetComponentInParent( entity );
		}

		template <class ComponentType>
		std::vector<ComponentType*> Transform::GetComponentsInChildren(const Entity* entity) const
		{
			return GetOwner( )->GetComponentsInChildren( entity );
		}

		template <class ComponentType>
		std::vector<ComponentType*> Transform::GetComponentsInParents(const Entity* entity) const
		{
			return GetOwner( )->GetComponentsInParents( entity );
		}
	}
}
