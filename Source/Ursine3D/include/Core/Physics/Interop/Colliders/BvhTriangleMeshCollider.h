/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BvhTriangleMeshCollider.h
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
		class BvhTriangleMeshCollider : public BvhTriangleMeshColliderBase
		{
		public:
			BvhTriangleMeshCollider(void);
			~BvhTriangleMeshCollider(void);

			void GenerateBvhTriangleMesh(ecs::Model3D *model);

			void SetScale(const SVec3 &scale);

			void Serialize(Json::object &output) const;
			void Deserialize(const Json &input);

		private:
		#ifdef BULLET_PHYSICS

			btTriangleIndexVertexArray *m_triangleIndexVertexArray;

		#endif
		};
	}
}
