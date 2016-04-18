/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** WaypointUtils.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <NativeJSFunction.h>

Meta(Enable, ExposeJavaScript)
JSFunction(WaypointUnidirectionalConnector);

Meta(Enable, ExposeJavaScript)
JSFunction(WaypointBidirectionalConnector);

Meta(Enable, ExposeJavaScript)
JSFunction(WaypointEnableDrawLines);

Meta(Enable, ExposeJavaScript)
JSFunction(WaypointDisableDrawLines);
