#include "UrsinePrecompiled.h"

#include "Spine/SpineAnimationConfig.h"

namespace ursine
{
    namespace spine
    {
        int BinarySearch(float *values, int valuesLength, float target, int step)
        {
            int low = 0, current;

            int high = valuesLength / step - 2;

            if (high == 0)
                return step;

            current = high >> 1;

            while (true)
            {
                if (values[(current + 1) * step] <= target)
                    low = current + 1;
                else
                    high = current;

                if (low == high)
                    return (low + 1) * step;

                current = (low + high) >> 1;
            }

            return 0;
        }

        int BinarySearch(float *values, int valuesLength, float target)
        {
            int low = 0, current;

            int high = valuesLength - 2;

            if (high == 0)
                return 1;

            current = high >> 1;

            while (true)
            {
                if (values[(current + 1)] <= target)
                    low = current + 1;
                else
                    high = current;

                if (low == high)
                    return low + 1;

                current = (low + high) >> 1;
            }

            return 0;
        }
    }
}