/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ContentManager.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ContentManager.h"

namespace Ursine
{
    ContentManager *gContentManager = nullptr;

    ContentManager::ContentManager(void)
    {
        gContentManager = this;
    }

    ContentManager::~ContentManager(void)
    {
        for (auto asset : _cache)
            delete asset.second;

        for (auto asset : _copies)
            delete asset;

        gContentManager = nullptr;
    }

    std::shared_ptr<std::string> ContentManager::LoadText(const std::string &name, uint32 flags)
    {
        std::ifstream stream(name, std::ios::in | flags);

        if (stream)
        {
            auto contents = std::make_shared<std::string>();

            stream.seekg(0, std::ios::end);

            contents->reserve(static_cast<std::string::size_type>(stream.tellg()));

            stream.seekg(0, std::ios::beg);

            contents->assign((std::istreambuf_iterator<char>(stream)), 
                std::istreambuf_iterator<char>());

            stream.close();

            return contents;
        }

        UError("Error loading text.\n- message: %s\n- file: %s", 
            strerror(errno), 
            name.c_str());

        return nullptr;
    }

    void ContentManager::Unload(const std::string &name)
    {
        auto asset = _cache.find(name);

        // this file doesn't actually exist
        if (asset == _cache.end())
            return;

        delete asset->second;

        _cache.erase(asset);
    }
}