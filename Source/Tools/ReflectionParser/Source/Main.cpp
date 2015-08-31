#include "Precompiled.h"

#include "ReflectionOptions.h"
#include "ReflectionParser.h"

int main(void)
{
    ReflectionOptions options;

    options.targetName = "Testing";

    options.sourceFile = "C:/Game Development/Ursine3D/Source/UrsineEngine/include/UrsinePrecompiled.h";

    options.arguments =
    { {
        "-x",
        "c++",
        "-std=c++11",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Debugging",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Entity",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Entity/Components",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Entity/Components/Core",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Entity/Managers",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Entity/Managers/Events",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Entity/Systems",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Entity/Systems/Core",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Entity/Systems/Core/Graphics",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Graphics",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Math",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Particles",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Particles/Effects",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Particles/Effects/Core",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Particles/Effects/Core/Data",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Physics/Collision",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Physics/Collision/Broad Phase",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Physics/Collision/Narrow Phase",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Physics/Collision/Shapes",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Physics/Common",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Physics/Debugging",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Physics/Dynamics",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Physics/Dynamics/Joints",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Spine/Attachments",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Spine/Timelines",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems/Audio",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems/Content",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems/Input",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems/Input/Gamepad",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems/Input/Keyboard",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems/Input/Mouse",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems/Screen",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems/Screen/Types",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems/UI",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems/UI/JavaScript",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems/UI/JavaScript/Extensions",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems/Utilities",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems/Utilities/Timer",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems/Utilities/Tween",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Systems/Utilities/Window",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Utilities",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Utilities/Events",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/include/Utilities/Serialization",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/dep/SDL2/include",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/dep/GLEW/include",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/dep/CEF",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/dep/CEF/include",
        "-IC:/Game Development/Ursine3D/Source/UrsineEngine/dep/FMOD/include",
    } };

    ReflectionParser parser( options );

    return 0;
}