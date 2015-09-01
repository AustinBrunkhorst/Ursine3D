
namespace ursine
{
    namespace ecs
    {
        template <typename PredicateFN>
        bool Simulation::RayCast(RayCastOutput &output, const RayCastInput &input,
                                 const LayerMask &mask, PredicateFN predicate)
        {
            float lowest_fraction = Math::MAX_FLOAT;
            bool hit_flag = false;
            RayCastOutput temp_output;
            output.fraction = input.max_fraction;

            // iterate through all bodies
            for (unsigned i = 0, size = _bodies.size(); i < size; ++i)
            {
                // if the body is inside the mask and we intersect with the AABB
                if (predicate(_bodies[i]) &&
                    mask.Contains(_bodies[i]->layer) &&
                    _bodies[i]->_aabb.RayCast(temp_output, input))
                {
                    // if we're intersecting with the collider inside of the AABB
                    if (ColliderRayCast(_bodies[i]->colliders, temp_output, input))
                    {
                        if (temp_output.fraction < output.fraction)
                        {
                            output = temp_output;

                            // set the new lowest fraction
                            lowest_fraction = output.fraction;

                            hit_flag = true;
                        }
                    }
                }
            }

            if (hit_flag)
            {
                // Let the debug drawer know about it
                _debug_drawer->addRayCast(output, input);
            }

            return hit_flag;
        }
    }
}
