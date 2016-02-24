#include "ResourcePipelineConfig.h"

///////////////////////////////////////////////////////////////////////////////
// Importers
///////////////////////////////////////////////////////////////////////////////

#include "TextureImporter.h"
#include "FBXImporter.h"
#include "AnimationClipImporter.h"
#include "JsonImporter.h"

///////////////////////////////////////////////////////////////////////////////
// Processors
///////////////////////////////////////////////////////////////////////////////

#include "PassThroughProcessor.h"
#include "TextureProcessor.h"
#include "WorldProcessor.h"

///////////////////////////////////////////////////////////////////////////////
// Resource Types
///////////////////////////////////////////////////////////////////////////////

#include "TextureData.h"
#include "TextureReader.h"

#include "ModelData.h"
#include "ModelReader.h"

#include "AnimationClipData.h"
#include "AnimationClipReader.h"

#include "JsonReader.h"
#include "JsonData.h"

#include "WorldReader.h"
#include "WorldData.h"