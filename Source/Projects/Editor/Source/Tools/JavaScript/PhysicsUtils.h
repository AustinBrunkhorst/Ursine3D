/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsUtils.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <NativeJSFunction.h>

Meta(Enable, ExposeJavaScript)
JSFunction(GenerateColliderForModel);

Meta(Enable, ExposeJavaScript)
JSFunction(ReduceConvexHull);

Meta(Enable, ExposeJavaScript)
JSFunction(GenerateCollidersForScene);

Meta(Enable, ExposeJavaScript)
JSFunction(GenerateBvhTriangleColliderForModel);

Meta(Enable, ExposeJavaScript)
JSFunction(GenerateBvhTriangleColliderForScene);
