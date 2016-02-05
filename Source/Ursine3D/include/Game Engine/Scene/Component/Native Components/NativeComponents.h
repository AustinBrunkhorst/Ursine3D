/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** NativeComponents.h
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

// All native components are to be added here

#include "RenderMask.h"

#include "TransformComponent.h"
#include "WorldConfigComponent.h"
#include "SelectedComponent.h"

///////////////////////////////////////////////////////////////////////////////
// Graphics
///////////////////////////////////////////////////////////////////////////////

#include "CameraComponent.h"
#include "Model3DComponent.h"
#include "LightComponent.h"
#include "Billboard2DComponent.h"
#include "AnimatorComponent.h"
#include "FBXSceneRootNodeComponent.h"
#include "ParticleSystemComponent.h"
#include "ParticleEmitterComponent.h"
#include "ParticleColorAnimatorComponent.h"
#include "ParticleAnimatorComponent.h"
#include "ParticleAttractorComponent.h"

///////////////////////////////////////////////////////////////////////////////
// Physics
///////////////////////////////////////////////////////////////////////////////

#include "RigidbodyComponent.h"
#include "BodyComponent.h"
#include "SphereColliderComponent.h"
#include "BoxColliderComponent.h"
#include "CylinderColliderComponent.h"
#include "CapsuleColliderComponent.h"
#include "ConeColliderComponent.h"
#include "ConvexHullColliderComponent.h"
#include "ConvexDecompColliderComponent.h"
#include "BvhTriangleMeshColliderComponent.h"
#include "EmptyColliderComponent.h"
#include "PhysicsSettingsComponent.h"

#include "SweptControllerComponent.h"

///////////////////////////////////////////////////////////////////////////////
// Audio
///////////////////////////////////////////////////////////////////////////////

#include "AudioEmitterComponent.h"
#include "ListenerComponent.h"
