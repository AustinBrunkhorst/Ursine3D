/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourcePipeline.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourcePipelineConfig.h"

///////////////////////////////////////////////////////////////////////////////
// Importers
///////////////////////////////////////////////////////////////////////////////

#include "TextureImporter.h"
#include "FontImporter.h"
#include "Content3DImporter.h"
#include "UModelImporter.h"
#include "AudioImporter.h"
#include "JsonImporter.h"
#include "UIProjectImporter.h"

///////////////////////////////////////////////////////////////////////////////
// Processors
///////////////////////////////////////////////////////////////////////////////

#include "PassThroughProcessor.h"
#include "TextureProcessor.h"
#include "WorldProcessor.h"
#include "ArchetypeProcessor.h"
#include "UIScreenProcessor.h"
#include "AudioItemProcessor.h"

///////////////////////////////////////////////////////////////////////////////
// Resource Types
///////////////////////////////////////////////////////////////////////////////

#include "TextureData.h"
#include "TextureReader.h"

#include "UModelData.h"
#include "UModelReader.h"
#include "URigData.h"
#include "URigReader.h"
#include "UAnimationData.h"
#include "UAnimationReader.h"

#include "FontData.h"
#include "FontReader.h"

#include "AnimationClipData.h"
#include "AnimationClipReader.h"

#include "AudioData.h"
#include "AudioReader.h"

#include "JsonReader.h"
#include "JsonData.h"

#include "WorldReader.h"
#include "WorldData.h"

#include "ArchetypeReader.h"
#include "ArchetypeData.h"

#include "UIProjectReader.h"
#include "UIProjectData.h"

#include "UIScreenReader.h"
#include "UIScreenData.h"

#include "AudioItemEventReader.h"
#include "AudioItemEventData.h"