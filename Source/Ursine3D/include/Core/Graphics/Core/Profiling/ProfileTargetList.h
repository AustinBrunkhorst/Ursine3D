#pragma once

//this MUST be in the order they are called
enum ProfilingTargets
{
  PROFILE_FRAME_BEGIN = 0,
  //leave above as first

  PROFILE_CLEAR_BUFFERS,
  PROFILE_DEFERRED,         //big (2nd)
  PROFILE_LIGHTS,           //big (1st)
  PROFILE_PRIMITIVES,
  PROFILE_DEBUG,
  PROFILE_SCENE_MAIN,       //big (3rd)
  PROFILE_SCENE_PRIMITIVE,
  PROFILE_SCENE_UI,
  

  //leave as last
  //PROFILE_FRAME_END,
  PROFILE_COUNT
};