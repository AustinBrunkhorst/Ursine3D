#pragma once

#include <Color.h>
#include <SMat4.h>

#include "GFXAPIDefines.h"

namespace ursine
{
    namespace graphics
    {
        class DrawingAPI
        {
            friend class GfxAPI;
        private:
            struct privData;

        public:
            // draw a point
            void DrawPoint(float x, float y, float z);
            void DrawPoint(const SVec3 &pos);

            // draw a line 
            void DrawLine(float x0, float y0, float z0, float x1, float y1, float z1);
            void DrawLine(const SVec3 &pos0, const SVec3 &pos1);

            // set render color
            void SetColor(float r, float g, float b, float a);
            void SetColor(const SVec4 &color);
            void SetColor(const Color &color);

            // size of the points, in pixels
            void SetSize(float size);

            //should this line/point ignore all depth?
            void SetOverdraw(bool draw);

            //get a texture of a certain size
            //resize texture
            //destroy texture
            //render a texture, this operates on a size/scale basis, based on the width/height originally defined

        private:
            privData *m_privates;

        private:
            void SetPrivates(void *priv);
            void Initialize();
            void Uninitialize();
        };
    }
}