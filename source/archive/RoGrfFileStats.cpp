#include "RoGrfFileStats.h"

namespace std
{
    void swap(RoGrfFileStats& stats1, RoGrfFileStats& stats2)
    {
        using std::swap;
        swap(stats1.compressedSize, stats2.compressedSize);
        swap(stats1.encryptedSize, stats2.encryptedSize);
        swap(stats1.originalSize, stats2.originalSize);
        swap(stats1.flags, stats2.flags);
        swap(stats1.offset, stats2.offset);
    }
}
