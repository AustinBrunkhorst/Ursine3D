/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorResourcePipelineManager.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <ResourcePipelineManager.h>

class Editor;
class Project;

class EditorResourcePipelineManager
{
public:
    EditorResourcePipelineManager(Project *project);
    ~EditorResourcePipelineManager(void);

private:
    Editor *m_editor;
    ursine::rp::ResourcePipelineManager &m_pipeline;

    ///////////////////////////////////////////////////////////////////////////
    // Resource Events
    ///////////////////////////////////////////////////////////////////////////

    void onResourceAdded(EVENT_HANDLER(ursine::rp::ResourcePipelineManager));
    void onResourceModified(EVENT_HANDLER(ursine::rp::ResourcePipelineManager));
};