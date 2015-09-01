#include "UrsinePrecompiled.h"

#include "Spine/Spine.h"

namespace ursine
{
    namespace spine
    {
        AtlasAttachmentLoader::AtlasAttachmentLoader(Atlas *atlas)
            : atlas(atlas)
        {

        }

        AtlasAttachmentLoader::~AtlasAttachmentLoader(void) { }

        Attachment *AtlasAttachmentLoader::NewAttachment(Skin *skin, AttachmentType type, const char *name, const char *path)
        {
            switch (type)
            {
            case SP_ATTACHMENT_REGION:
            {
                RegionAttachment *attachment;
                AtlasRegion *region = atlas->FindRegion(path);

                UAssert(region, "Region not found: %s", path);

                attachment = new RegionAttachment(name);

                attachment->renderer_object = region;

                attachment->SetUVs(region->u, region->v, region->u2, region->v2, region->rotate);

                attachment->region_offset_x = region->offset_x;
                attachment->region_offset_y = region->offset_y;
                attachment->region_width = region->width;
                attachment->region_height = region->height;
                attachment->region_original_width = region->original_width;
                attachment->region_original_height = region->original_height;

                return attachment;
            }
            case SP_ATTACHMENT_MESH:
            {
                MeshAttachment *attachment;

                AtlasRegion *region = atlas->FindRegion(path);

                UAssert(region, "Region not found: %s", path);

                attachment = new MeshAttachment(name);

                attachment->renderer_object = region;
                attachment->region_u = region->u;
                attachment->region_v = region->v;
                attachment->region_u2 = region->u2;
                attachment->region_v2 = region->v2;
                attachment->region_rotate = region->rotate;
                attachment->region_offset_x = region->offset_x;
                attachment->region_offset_y = region->offset_y;
                attachment->region_width = region->width;
                attachment->region_height = region->height;
                attachment->region_original_width = region->original_width;
                attachment->region_original_height = region->original_height;

                return attachment;
            }
            case SP_ATTACHMENT_SKINNED_MESH:
            {
                SkinnedMeshAttachment *attachment;

                AtlasRegion *region = atlas->FindRegion(path);

                UAssert(region, "Region not found: %s", path);

                attachment = new SkinnedMeshAttachment(name);

                attachment->renderer_object = region;
                attachment->region_u = region->u;
                attachment->region_v = region->v;
                attachment->region_u2 = region->u2;
                attachment->region_v2 = region->v2;
                attachment->region_rotate = region->rotate;
                attachment->region_offset_x = region->offset_x;
                attachment->region_offset_y = region->offset_y;
                attachment->region_width = region->width;
                attachment->region_height = region->height;
                attachment->region_original_width = region->original_width;
                attachment->region_original_height = region->original_height;

                return attachment;
            }
            case SP_ATTACHMENT_BOUNDING_BOX:
                return new BoundingBoxAttachment(name);
            default:
                UError("Unknown attachment type: %i", type);
                return nullptr;
            }
        }
    }
}