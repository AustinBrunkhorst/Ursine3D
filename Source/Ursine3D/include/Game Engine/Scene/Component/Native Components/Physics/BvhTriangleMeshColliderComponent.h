/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BvhTriangleMeshColliderComponent.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "BvhTriangleMeshCollider.h"

namespace ursine
{
	namespace ecs
	{
		class BvhTriangleMeshCollider : public Component
		{
			NATIVE_COMPONENT;

			friend class PhysicsSystem;

		public:
			BvhTriangleMeshCollider(void);
			~BvhTriangleMeshCollider(void);

			Meta(Disable)
			void OnInitialize(void) override;

			Meta(Disable)
			void OnSerialize(Json::object &output) const override;

			Meta(Disable)
			void OnDeserialize(const Json &input) override;

			void GenerateBvhTriangleMesh(Model3D *model);

		private:
			physics::BvhTriangleMeshCollider m_bvhTriangleMeshCollider;

			void onTransformChange(EVENT_HANDLER(Entity));

		} Meta( Enable, DisplayName( "BvhTriangleMeshCollider" ) );
	}
}
