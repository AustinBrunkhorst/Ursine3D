#include "UrsinePrecompiled.h"

#include "Spine/Spine.h"

#include <ctype.h>

namespace ursine
{
    namespace spine
    {
        struct Str
        {
            const char *begin;
            const char *end;
        };

        static void trim(Str *str)
        {
            while (isspace(*str->begin) && str->begin < str->end)
                (str->begin)++;

            if (str->begin == str->end)
                return;

            str->end--;

            while (isspace(*str->end) && str->end >= str->begin)
                str->end--;

            str->end++;
        }

        /* Tokenize string without modification. Returns 0 on failure. */
        static int readLine(const char *begin, const char *end, Str *str)
        {
            static const char *nextStart;

            if (begin)
            {
                nextStart = begin;
                return 1;
            }

            if (nextStart == end)
                return 0;

            str->begin = nextStart;

            /* Find next delimiter. */
            while (nextStart != end && *nextStart != '\n')
                nextStart++;

            str->end = nextStart;

            trim(str);

            if (nextStart != end)
                nextStart++;

            return 1;
        }

        /* Moves str->begin past the first occurence of c. Returns 0 on failure. */
        static int beginPast(Str *str, char c)
        {
            const char *begin = str->begin;

            while (1)
            {
                char lastSkippedChar = *begin;

                if (begin == str->end)
                    return 0;

                begin++;

                if (lastSkippedChar == c)
                    break;
            }

            str->begin = begin;

            return 1;
        }

        /* Returns 0 on failure. */
        static int readValue(const char *end, Str *str)
        {
            readLine(nullptr, end, str);

            if (!beginPast(str, ':'))
                return 0;

            trim(str);

            return 1;
        }

        /* Returns the number of tuple values read (1, 2, 4, or 0 for failure). */
        static int readTuple(const char *end, Str tuple[])
        {
            int i;

            Str str = { NULL, NULL };

            readLine(nullptr, end, &str);

            if (!beginPast(&str, ':'))
                return 0;

            for (i = 0; i < 3; ++i)
            {
                tuple[i].begin = str.begin;

                if (!beginPast(&str, ','))
                    break;

                tuple[i].end = str.begin - 2;

                trim(&tuple[i]);
            }

            tuple[i].begin = str.begin;
            tuple[i].end = str.end;

            trim(&tuple[i]);

            return i + 1;
        }

        static char *mallocString(Str *str)
        {
            int length = (int)(str->end - str->begin);

            char *string = new char[length + 1];

            memcpy(string, str->begin, length);

            string[length] = '\0';

            return string;
        }

        static int indexOf(const char **array, int count, Str *str)
        {
            int length = (int)(str->end - str->begin);

            int i;

            for (i = count - 1; i >= 0; i--)
                if (strncmp(array[i], str->begin, length) == 0)
                    return i;

            return -1;
        }

        static bool equals(Str *str, const char *other)
        {
            return strncmp(other, str->begin, str->end - str->begin) == 0;
        }

        static int toInt(Str *str)
        {
            return (int)strtol(str->begin, (char**)&str->end, 10);
        }

        static const char *formatNames[] = {
            "Alpha",
            "Intensity",
            "LuminanceAlpha",
            "RGB565",
            "RGBA4444",
            "RGB888",
            "RGBA8888"
        };

        static const char *textureFilterNames[] = {
            "Nearest",
            "Linear",
            "MipMap",
            "MipMapNearestNearest",
            "MipMapLinearNearest",
            "MipMapNearestLinear",
            "MipMapLinearLinear"
        };

        AtlasPage::AtlasPage(Atlas *atlas, const char *name)
            : atlas(atlas)
            , name(name)
        {

        }

        AtlasPage::~AtlasPage(void)
        {
            // normally dispose of texture, but the content manager handles that
        }

        AtlasRegion::AtlasRegion(void)
        {

        }

        AtlasRegion::~AtlasRegion(void)
        {
            delete []name;
            delete []splits;
            delete []pads;
        }

        Atlas::Atlas(const char *begin, int length, const char *dir)
        {
            int count;
            const char *end = begin + length;
            int dirLength = (int)strlen(dir);
            int needsSlash = dirLength > 0 && dir[dirLength - 1] != '/' && dir[dirLength - 1] != '\\';

            AtlasPage *page = 0;
            AtlasPage *lastPage = 0;
            Str str;
            Str tuple[4];

            readLine(begin, nullptr, nullptr);

            while (readLine(nullptr, end, &str))
            {
                if (str.end - str.begin == 0)
                {
                    page = nullptr;
                }
                else if (!page)
                {
                    char *name = mallocString(&str);
                    char *path = new char[dirLength + needsSlash + strlen(name) + 1];
                    memcpy(path, dir, dirLength);
                    if (needsSlash)
                        path[dirLength] = '/';
                    strcpy(path + dirLength + needsSlash, name);

                    page = new AtlasPage(this, name);

                    delete []name;

                    if (lastPage)
                        lastPage->next = page;
                    else
                        pages = page;

                    lastPage = page;

                    switch (readTuple(end, tuple))
                    {
                    case 0:
                        UError("Invalid tuple (empty).");
                    case 2: /* size is only optional for an atlas packed with an old TexturePacker. */
                        page->width = toInt(tuple);
                        page->height = toInt(tuple + 1);

                        UAssert(readTuple(end, tuple), "Invalid tuple.");
                    }

                    page->format = (AtlasFormat)indexOf(formatNames, 7, tuple);

                    UAssert(readTuple(end, tuple), "Invalid tuple.");

                    page->min_filter = (AtlasFilter)indexOf(textureFilterNames, 7, tuple);
                    page->mag_filter = (AtlasFilter)indexOf(textureFilterNames, 7, tuple + 1);

                    UAssert(readValue(end, &str), "Invalid tuple.");

                    if (!equals(&str, "none"))
                    {
                        page->u_wrap = *str.begin == 'x' ? SP_ATLAS_REPEAT : (*str.begin == 'y' ? SP_ATLAS_CLAMPTOEDGE : SP_ATLAS_REPEAT);
                        page->v_wrap = *str.begin == 'x' ? SP_ATLAS_CLAMPTOEDGE : (*str.begin == 'y' ? SP_ATLAS_REPEAT : SP_ATLAS_REPEAT);
                    }

                    page->texture = ursine::gContentManager->Load<ursine::Texture2D>(path);

                    auto size = page->texture->GetSize();

                    page->width = static_cast<int>(size.X());
                    page->height = static_cast<int>(size.Y());

                    delete []path;
                }
                else
                {
                    AtlasRegion *region = new AtlasRegion();

                    region->page = page;
                    region->name = mallocString(&str);

                    URSINE_TODO("test this");

                    regions[std::string(str.begin, str.end)] = region;

                    UAssert(readValue(end, &str), "Invalid format.");

                    region->rotate = equals(&str, "true");

                    UAssert(readTuple(end, tuple) == 2, "Invalid format.");

                    region->x = toInt(tuple);
                    region->y = toInt(tuple + 1);

                    UAssert(readTuple(end, tuple) == 2, "Invalid format.");

                    region->width = toInt(tuple);
                    region->height = toInt(tuple + 1);

                    region->u = region->x / (float)page->width;
                    region->v = region->y / (float)page->height;

                    if (region->rotate)
                    {
                        region->u2 = (region->x + region->height) / (float)page->width;
                        region->v2 = (region->y + region->width) / (float)page->height;
                    }
                    else
                    {
                        region->u2 = (region->x + region->width) / (float)page->width;
                        region->v2 = (region->y + region->height) / (float)page->height;
                    }

                    UAssert(count = readTuple(end, tuple), "Invalid format.");

                    if (count == 4)
                    { /* split is optional */
                        region->splits = new int[4];
                        region->splits[0] = toInt(tuple);
                        region->splits[1] = toInt(tuple + 1);
                        region->splits[2] = toInt(tuple + 2);
                        region->splits[3] = toInt(tuple + 3);

                        UAssert(count = readTuple(end, tuple), "Invalid format.");

                        if (count == 4)
                        { /* pad is optional, but only present with splits */
                            region->pads = new int[4];
                            region->pads[0] = toInt(tuple);
                            region->pads[1] = toInt(tuple + 1);
                            region->pads[2] = toInt(tuple + 2);
                            region->pads[3] = toInt(tuple + 3);

                            UAssert(readTuple(end, tuple), "Invalid format.");
                        }
                    }

                    region->original_width = toInt(tuple);
                    region->original_height = toInt(tuple + 1);

                    readTuple(end, tuple);

                    region->offset_x = toInt(tuple);
                    region->offset_y = toInt(tuple + 1);

                    UAssert(readValue(end, &str), "Invalid format.");

                    region->index = toInt(&str);
                }
            }
        }

        Atlas::~Atlas(void)
        {
            AtlasPage *page = pages;

            while (page)
            {
                AtlasPage *next_page = page->next;

                delete page;

                page = next_page;
            }

            for (auto region : regions)
                delete region.second;
        }

        Atlas *Atlas::CreateFromFile(const char *path)
        {
            auto normalized = ursine::utils::NormalizePath(path);

            auto dir = ursine::utils::DirectoryName(normalized);

            auto data = ursine::gContentManager->LoadText(normalized);

            return new Atlas(data->c_str(), data->length(), dir.c_str());
        }

        AtlasRegion *Atlas::FindRegion(const char *name)
        {
            return regions[name];
        }
    }
}
