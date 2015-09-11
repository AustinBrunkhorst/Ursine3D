#pragma once

namespace ursine
{
    namespace spine
    {
        int BinarySearch(float *values, int values_length, float target, int step);

        int BinarySearch(float *values, int values_length, float target);

        const float CURVE_LINEAR = 0;
        const float CURVE_STEPPED = 1;
        const float CURVE_BEZIER = 2;
        const int BEZIER_SEGMENTS = 10;
        const int BEZIER_SIZE = 10 * 2 - 1;

        const int ROTATE_PREV_FRAME_TIME = -2;
        const int ROTATE_FRAME_VALUE = 1;

        const int TRANSLATE_PREV_FRAME_TIME = -3;
        const int TRANSLATE_FRAME_X = 1;
        const int TRANSLATE_FRAME_Y = 2;

        const int COLOR_PREV_FRAME_TIME = -5;
        const int COLOR_FRAME_R = 1;
        const int COLOR_FRAME_G = 2;
        const int COLOR_FRAME_B = 3;
        const int COLOR_FRAME_A = 4;

        const int IKCONSTRAINT_PREV_FRAME_TIME = -3;
        const int IKCONSTRAINT_PREV_FRAME_MIX = -2;
        const int IKCONSTRAINT_PREV_FRAME_BEND_DIRECTION = -1;
        const int IKCONSTRAINT_FRAME_MIX = 1;
    }
}
