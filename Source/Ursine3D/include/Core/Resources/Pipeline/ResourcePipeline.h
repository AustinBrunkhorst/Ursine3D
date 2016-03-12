#include "ResourcePipelineConfig.h"

///////////////////////////////////////////////////////////////////////////////
// Importers
///////////////////////////////////////////////////////////////////////////////

#include "TextureImporter.h"
#include "FontImporter.h"
#include "FBXFileImporter.h"
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

///////////////////////////////////////////////////////////////////////////////
// Resource Types
///////////////////////////////////////////////////////////////////////////////

#include "TextureData.h"
#include "TextureReader.h"

#include "ModelData.h"
#include "ModelReader.h"

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