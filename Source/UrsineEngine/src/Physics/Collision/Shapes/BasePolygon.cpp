/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BasePolygon.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

namespace ursine
{
    BasePolygon::BasePolygon(const std::string &path, const std::vector<Vec2> &points, VERTEX_ORDER vert_order)
        : Asset(path)
    {
        vertices = points;

        // compute normals
        if (vert_order == VERTEX_ORDER_CCW)
        {
            SMat3 rot = SMat3(-math::PI_DIV_2);
            for (uint32 i = 0; i < points.size(); ++i)
            {

                Vec2 normal;

                if (i == points.size() - 1)
                    normal = points[0] - points[i];
                else
                    normal = points[i + 1] - points[i];

                normal = rot.TransformVector(normal);
                normals.push_back(Vec2::Normalize(normal));
            }
        }
        else
        {
            SMat3 rot = SMat3(math::PI_DIV_2);
            for (uint32 i = 0; i < points.size(); ++i)
            {
                Vec2 normal;

                if (i == points.size() - 1)
                    normal = points[0] - points[i];
                else
                    normal = points[i + 1] - points[i];

                normal = rot.TransformVector(normal);
                normals.push_back(Vec2::Normalize(normal));
            }
        }
    }

    const BasePolygon *BasePolygon::Get(const std::string &path)
    {
        return gContentManager->Load<BasePolygon>(path);
    }

    template<>
    BasePolygon *ContentManager::Loader(const std::string &name, uint32 flags)
    {
        std::string error;
        std::string file_name = name + ".mesh";

        auto text = LoadText(file_name);

        Json mesh = Json::parse(*text, error);

        UAssert(error.empty(), 
            "Error parsing Mesh (%s).\n%s", name.c_str(), error.c_str());

        // this is where we store out verts
        std::vector<Vec2> verts;

        for (auto &vector : mesh.array_items())
        {
            float x = static_cast<float>(vector[0].number_value());
            float y = static_cast<float>(vector[1].number_value());

            verts.emplace_back(x, y);
        }

        return new BasePolygon(name, verts);
    }
}