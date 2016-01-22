/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ConvexHullCollider.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "PhysicsInteropConfig.h"

namespace ursine
{
	namespace ecs
	{
		class Model3D;
	}

	namespace physics
	{
		class ConvexHullCollider : public ConvexHullColliderBase
		{
		public:
			ConvexHullCollider(void);
			~ConvexHullCollider(void);

			void GenerateConvexHull(ecs::Model3D *model);

			void ReduceVertices(void);

			float GetMargin(void) const;
			void SetMargin(float margin);

			void SetScale(const SVec3 &scale);

			void Serialize(Json::object &output) const;
			void Deserialize(const Json &input);

			void ToggleDebugDraw(bool toggle);
		};
	}
}
