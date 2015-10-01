#pragma once

#include <Scene.h>

class Project
{
public:
    ursine::Scene &GetScene(void);

private:
    ursine::Scene m_scene;
};