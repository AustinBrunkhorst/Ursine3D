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
  RASTER_STATE_SOLID_BACKCULL = 0,
  RASTER_STATE_SOLID_FRONTCULL,
  RASTER_STATE_SOLID_NOCULL,
  RASTER_STATE_WIREFRAME_BACKCULL,
  RASTER_STATE_WIREFRAME_FRONTCULL,
  RASTER_STATE_WIREFRAME_NOCULL,
  RASTER_STATE_LINE_RENDERING,
  RASTER_STATE_UI,
  RASTER_STATE_COUNT
};