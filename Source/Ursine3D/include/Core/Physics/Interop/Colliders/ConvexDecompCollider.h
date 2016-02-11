/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ConvexDecompCollider.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "PhysicsInteropConfig.h"
#include "ConvexHullCollider.h"

namespace ursine
{
	namespace ecs
	{
		class Model3D;
	}

	namespace physics
	{
		class ConvexDecompCollider : public ConvexDecompColliderBase
		{
		public:
			ConvexDecompCollider(void);
			~ConvexDecompCollider(void);

			void GenerateConvexHulls(ecs::Model3D *model);

			void Serialize(Json::object &output) const;
			void Deserialize(const Json &input);

			void SetScale(const SVec3 &scale);

			float GetMargin(void) const;
			void SetMargin(float margin);

		private:
			void clearConvexHulls(void);
		};
	}
}