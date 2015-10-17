/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsInteropConfig.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#define BULLET_PHYSICS

#ifdef BULLET_PHYSICS

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletWorldImporter/btWorldImporter.h"

typedef btIDebugDraw PhysicsDebugDraw;

#endif
