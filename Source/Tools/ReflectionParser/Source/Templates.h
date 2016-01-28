/* ----------------------------------------------------------------------------
** Team Bear King
** © 201x DigiPen Institute of Technology, All Rights Reserved.
**
** Templates.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#define EXT ".mustache"

const auto kTemplateModuleHeader                 = "module-header" EXT;
const auto kTemplateModuleSource                = "module-source" EXT;
const auto kTemplateModuleFileHeader            = "module-file-header" EXT;
const auto kTemplateModuleFileSource            = "module-file-source" EXT;
                                                
const auto kPartialFunctionInvocation           = "function-invocation" EXT;
const auto kPartialConstructorInvocation        = "constructor-invocation" EXT;
const auto kPartialDynamicConstructorInvocation = "dynamic-constructor-invocation" EXT;
const auto kPartialMethodInvocation             = "method-invocation" EXT;
                                                
const auto kPartialGlobalGetter                 = "global-getter" EXT;
const auto kPartialGlobalSetter                 = "global-setter" EXT;
                                                
const auto kPartialFieldGetter                  = "field-getter" EXT;
const auto kPartialFieldSetter                  = "field-setter" EXT;
                                                
const auto kPartialMetaInitializerList          = "meta-initializer-list" EXT;

#undef EXT