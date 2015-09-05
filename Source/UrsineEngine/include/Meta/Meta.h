#pragma once

#include <Macros.h>

#ifdef __REFLECTION_PARSER__

#include <Object.h>
#include <MetaProperty.h>

#else

#include <Object.h>

#include <Variant.h>

#include <Type.h>

#include <MetaManager.h>

#include <Enum.h>
#include <Constructor.h>
#include <Destructor.h>
#include <Field.h>
#include <Method.h>
#include <Function.h>

#endif