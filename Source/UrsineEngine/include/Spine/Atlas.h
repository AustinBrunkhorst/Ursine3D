#pragma once

#include "Texture2D.h"
#include "AtlasConfig.h"

namespace Ursine
{
    class SkeletonRenderer;

    namespace Spine
    {
        class AtlasPage
        {
            friend class Atlas;
            friend SkeletonRenderer;

            const Atlas *atlas;
            std::string name;
            AtlasFormat format;

            AtlasFilter min_filter;
            AtlasFilter mag_filter;

            AtlasWrap u_wrap;
            AtlasWrap v_wrap;

            Ursine::Texture2D *texture;

            int width;
            int height;

            AtlasPage *next;

        public:
            AtlasPage(Atlas *atlas, const char *name);
            ~AtlasPage(void);
        };

        class AtlasRegion
        {
            friend class Atlas;
            friend class AtlasAttachmentLoader;
            friend SkeletonRenderer;

            const char *name;
            int x;
            int y;
            int width;
            int height;

            float u;
            float v;
            float u2;
            float v2;

            int offset_x;
            int offset_y;

            int original_width;
            int original_height;

            int index;
            bool rotate;
            bool flip;
            int *splits;
            int *pads;

            AtlasPage *page;

            AtlasRegion *next;

        public:
            AtlasRegion(void);
            ~AtlasRegion(void);
        };

        class Atlas
        {
            AtlasPage *pages;
            std::unordered_map<std::string, AtlasRegion *> regions;

        public:
            Atlas(const char *begin, int length, const char *dir);
            ~Atlas(void);

            static Atlas *CreateFromFile(const char *path);

            AtlasRegion *FindRegion(const char *name);
        };
    }
}
