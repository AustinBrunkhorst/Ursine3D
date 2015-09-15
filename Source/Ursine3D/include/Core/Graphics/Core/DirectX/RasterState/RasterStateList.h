/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      RasterStateList.h
Module:         Graphics
Purpose:        enums of raster states
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

enum RASTER_STATES
{
  RASTER_STATE_BACKFACE_CULL = 0,
  RASTER_STATE_FRONTFACE_CULL,
  RASTER_STATE_NO_CULL,
  RASTER_STATE_WIREFRAME_BACKFACE_CULL,
  RASTER_STATE_WIREFRAME_FRONTFACE_CULL,
  RASTER_STATE_WIREFRAME_NO_CULL,
  RASTER_STATE_LINE_RENDERING,
  RASTER_STATE_UI,
  RASTER_STATE_COUNT
};