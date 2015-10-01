/* ----------------------------------------------------------------------------
** Team Bear King
** © 201x DigiPen Institute of Technology, All Rights Reserved.
**
** MetaDataConfig.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

namespace native_property
{
    const auto Enable = "Enable";
    const auto Disable = "Disable";

    const auto Register = "Register";

    const auto DisplayName = "DisplayName";

    const auto DisablePtrType = "DisablePtrType";
    const auto DisableConstPtrType = "DisableConstPtrType";

    const auto DisableNonDynamicCtor = "DisableNonDynamic";
    const auto DynamicCtorWrap = "WrapObject";

    const auto ExplicitGetter = "Getter";
    const auto ExplicitSetter = "Setter";
}

const auto kMetaExternalTypeDefName = "__META_EXTERNAL__";