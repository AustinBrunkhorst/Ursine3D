// Steps to register your particle initializer:
// 1. Create a macro below with the desired serialization string name
// 2. Add your new header file to "ParticleInitializers.h"
// 3. Add an if case for your class at the bottom of "ParticleEmitterComponent.cpp"
//    inside the deserialize function.

//These message names are processed inside ParticleInitializers.h
REGISTER_INITIALIZER_NAME(LifeSpan)
REGISTER_INITIALIZER_NAME(PositionSetter)
REGISTER_INITIALIZER_NAME(ScaleSetter)
REGISTER_INITIALIZER_NAME(MoveToScale)
REGISTER_INITIALIZER_NAME(MoveToPosition)
REGISTER_INITIALIZER_NAME(Orbit)
REGISTER_INITIALIZER_NAME(ColorSetter)
REGISTER_INITIALIZER_NAME(MoveToColor)
REGISTER_INITIALIZER_NAME(Beam)
REGISTER_INITIALIZER_NAME(TextureSetter)
REGISTER_INITIALIZER_NAME(MoveToCircle)
