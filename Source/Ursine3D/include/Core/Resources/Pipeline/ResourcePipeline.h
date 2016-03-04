#include "ResourcePipelineConfig.h"

///////////////////////////////////////////////////////////////////////////////
// Importers
///////////////////////////////////////////////////////////////////////////////

#include "TextureImporter.h"
#include "FBXFileImporter.h"
#include "AnimationClipImporter.h"
#include "AudioImporter.h"
#include "JsonImporter.h"

///////////////////////////////////////////////////////////////////////////////
// Processors
///////////////////////////////////////////////////////////////////////////////

#include "PassThroughProcessor.h"
#include "TextureProcessor.h"
#include "WorldProcessor.h"
#include "ArchetypeProcessor.h"

///////////////////////////////////////////////////////////////////////////////
// Resource Types
///////////////////////////////////////////////////////////////////////////////

#include "TextureData.h"
#include "TextureReader.h"

#include "ModelData.h"
#include "ModelReader.h"

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