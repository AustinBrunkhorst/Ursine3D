#pragma once

enum ViewportRenderMode
{
  VIEWPORT_RENDER_FORWARD = 0,  //forward rendering, camera-based lighting, super fast! NOTE: Lights will not work
  VIEWPORT_RENDER_DEFERRED,     //deferred rendering, cool point lights! Expensive

  RENDER_COUNT
};