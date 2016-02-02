/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ConvexDecompColliderComponent.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "ConvexDecompCollider.h"

namespace ursine
{
	namespace ecs
	{
		class ConvexDecompCollider : public Component
		{
			NATIVE_COMPONENT;

			friend class PhysicsSystem;

		public:

			EditorField(
				float margin,
				GetMargin,
				SetMargin
			);

			ConvexDecompCollider(void);
			~ConvexDecompCollider(void);

			Meta(Disable)
			void OnInitialize(void) override;

			Meta(Disable)
			void OnSerialize(Json::object &output) const override;

			Meta(Disable)
			void OnDeserialize(const Json &input) override;

			void GenerateConvexHulls(ursine::ecs::Model3D *model);

			float GetMargin(void) const;
			void SetMargin(float margin);

		private:
			mutable physics::ConvexDecompCollider m_convexDecompCollider;

			void onTransformChange(EVENT_HANDLER(Entity));

		} Meta( Enable, DisplayName( "ConvexDecompCollider" ) );
	}
}
