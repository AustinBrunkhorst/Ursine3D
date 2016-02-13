namespace ursine
{
    namespace resources
    {
        template<typename ResourceType>
        ResourceType *ResourceManager::Load(const std::string &resourceName)
        {
            return nullptr;
        }

        template<typename ResourceType>
        ResourceType *ResourceManager::LoadReference(const ResourceReference &reference)
        {
            return nullptr;
        }
    }
}