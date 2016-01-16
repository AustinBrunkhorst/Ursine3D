/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ConvexHullColliderComponent.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "ConvexHullCollider.h"

namespace ursine
{
	namespace ecs
	{
		class ConvexHullCollider : public Component
		{
			NATIVE_COMPONENT;

			friend class PhysicsSystem;

		public:

			EditorField(
				float margin,
				GetMargin,
				SetMargin
			);

			ConvexHullCollider(void);
			~ConvexHullCollider(void);

			Meta(Disable)
			void OnInitialize(void) override;

			void GenerateConvexHull(Model3D *model);

			void ReduceVertices(void);

			float GetMargin(void) const;
			void SetMargin(float margin);

		private:
			physics::ConvexHullCollider m_convexHullCollider;

		} Meta( Enable, DisplayName( "ConvexHullCollider" ) );
	}
}
