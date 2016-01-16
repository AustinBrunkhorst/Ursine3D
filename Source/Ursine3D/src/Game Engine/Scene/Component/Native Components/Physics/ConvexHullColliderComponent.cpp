/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ConvexHullColliderComponent.cpp
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ConvexHullColliderComponent.h"

namespace ursine
{
	namespace ecs
	{
		NATIVE_COMPONENT_DEFINITION(ConvexHullCollider);


		ConvexHullCollider::ConvexHullCollider(void)
			: BaseComponent( )
		{
		}

		ConvexHullCollider::~ConvexHullCollider(void)
		{
		}

		void ConvexHullCollider::OnInitialize(void)
		{
		}

		void ConvexHullCollider::GenerateConvexHull(Model3D* model)
		{
			m_convexHullCollider.GenerateConvexHull( model );
		}

		void ConvexHullCollider::ReduceVertices(void)
		{
			m_convexHullCollider.ReduceVertices( );
		}

		float ConvexHullCollider::GetMargin(void) const
		{
			return m_convexHullCollider.GetMargin( );
		}

		void ConvexHullCollider::SetMargin(float margin)
		{
			if (margin < 0.0f)
				return;

			m_convexHullCollider.SetMargin( margin );
		}
	}
}
