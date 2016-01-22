/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorTool.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <KeyboardManager.h>
#include <MouseManager.h>

class EditorTool
{
public:
	virtual void OnEnable(void);
	virtual void OnDisable(void);

	virtual void OnUpdate(ursine::KeyboardManager *kManager, ursine::MouseManager *mManager);
};
