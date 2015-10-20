#define CATCH_CONFIG_RUNNER
#include "../dep/Testing/include/catch.hpp"

#include <EntityManager.h>
#include <Application.h>
#include <World.h>
#include <TransformComponent.h>
#include <SVec3.h>

using namespace ursine;
using namespace ecs;

int main(int argc, char * argv[])
{
    Application application(argc, argv);

    int result = Catch::Session().run(argc, argv);

    return result;
}

TEST_CASE("Hi I'm A Test Case")
{
    World world;

    auto e = world.CreateEntity();
    auto t = e->GetComponent<Transform>();

    REQUIRE(t->GetWorldPosition() == SVec3(0.0f, 0.0f, 0.0f));
}
