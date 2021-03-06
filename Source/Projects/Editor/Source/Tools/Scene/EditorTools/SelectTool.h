/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SelectTool.h
**
** Authors:
** - Jordan Ellis - j.ellis@digipen.edu
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "EditorTool.h"

class SelectTool : public EditorTool
{
public:
    SelectTool(Editor *editor, ursine::ecs::World *world);
    ~SelectTool(void);

    void OnMouseDown(const ursine::MouseButtonArgs &args) override;

    void OnSelect(const ursine::ecs::EntityHandle &selected) override;

    void OnUpdate(ursine::KeyboardManager *kManager, ursine::MouseManager *mManager) override;

    void unpickObject(void);

private:
    ursine::graphics::GfxAPI *m_graphics;

    ursine::ecs::EntityHandle m_currentEntity;

    bool m_altDown;
};
