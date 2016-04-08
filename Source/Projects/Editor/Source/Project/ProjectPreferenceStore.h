#pragma once

class Project;

class ProjectPreferenceStore
{
public:
    ProjectPreferenceStore(Project *project);

    const ursine::Json &Load(
        const std::string &category,
        const std::string &item,
        const ursine::Json &defaultValue = ursine::Json( )
    );

    void Store(
        const std::string &category, 
        const std::string &item, 
        const ursine::Json &value
    );
private:
    typedef std::map<std::string, ursine::Json> Category;

    Project *m_project;

    std::unordered_set<std::string> m_loadedCategories;
    std::unordered_map<std::string, Category> m_store;

    std::mutex m_storeMutex;

    void checkCategoryLoaded(const std::string &category);

    void loadCategory(const std::string &category);
    void storeCategory(const std::string &category);

    ursine::fs::path getCategoryPath(const std::string &category);
};