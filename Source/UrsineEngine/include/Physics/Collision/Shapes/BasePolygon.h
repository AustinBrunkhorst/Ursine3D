/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BasePolygon.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Vector2.h"
#include "ContentManager.h"

#include <vector>
#include <unordered_map>

namespace Ursine
{
    enum VERTEX_ORDER
    {
        VERTEX_ORDER_CW,
        VERTEX_ORDER_CCW
    };

    class BasePolygon : public Asset
    {
    public:
        static const BasePolygon *Get(const std::string &path);

        std::vector<Vector2> vertices;
        std::vector<Vector2> normals;

        BasePolygon(const std::string &path,
                    const std::vector<Vector2> &points,
                    VERTEX_ORDER vert_order = VERTEX_ORDER_CCW);
    };

    extern template
    BasePolygon *ContentManager::Loader(const std::string &name, uint32 flags);
}