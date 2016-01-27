namespace utils
{
    template<typename A, typename B>
    bool RangeEqual(A startA, A endA, B startB, B endB)
    {
        while (startA != endA && startB != endB)
        {
            if (*startA != *startB)
                return false;

            ++startA;
            ++startB;
        }

        return (startA == endA) && (startB == endB);
    }
}