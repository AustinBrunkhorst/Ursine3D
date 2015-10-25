#pragma once

class Project;

class EditorEntityManager
{
public:
    EditorEntityManager(std::shared_ptr<Project> project);

private:
    std::shared_ptr<Project> m_project;
};