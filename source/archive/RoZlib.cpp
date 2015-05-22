#include "RoZlib.h"

#include <zlib.h>

#include <core/RoException.h>
#include <core/RoErrorInfo.h>

RoErrorInfoDetail GetZlibErrorDetail(int errorCode)
{
    switch (errorCode) {
    case Z_STREAM_END:
        return RoErrorInfoDetail("Reached end of stream.");
        break;
    case Z_NEED_DICT:
        return RoErrorInfoDetail("Need dict(?)");
        break;
    case Z_ERRNO:
        return RoErrorInfoDetail("ERRNO.");
        break;
    case Z_STREAM_ERROR:
        return RoErrorInfoDetail("Stream error.");
        break;
    case Z_DATA_ERROR:
        return RoErrorInfoDetail("Source data error.");
        break;
    case Z_MEM_ERROR:
        return RoErrorInfoDetail("Memory Error.");
        break;
    case Z_BUF_ERROR:
        return RoErrorInfoDetail("Unclear buffer error.");
        break;
    case Z_VERSION_ERROR:
        return RoErrorInfoDetail("Incompatible zlib version.");
        break;
    default:
        break;
    }
    return RoErrorInfoDetail("Unknown zlib error.");
}

void RoZlib::Uncompress(RoCharArray& destination, const size_t uncompressedSize, const RoCharArray& source)
{
    RoCharArray uncompressed(uncompressedSize, 0);
    uLong resultingUncompressedSize = static_cast<uLong>(uncompressedSize);

    int result = uncompress(
        (Bytef*) &uncompressed[0],
        &resultingUncompressedSize,
        (const Bytef*) &source[0],
        static_cast<uLong>(source.size()));

    if (Z_OK != result) {
        roTHROW(RoException("ZlibException")
                << GetZlibErrorDetail(result));
    } else if (uncompressedSize != resultingUncompressedSize) {
        roTHROW(RoException("ZlibException")
                << RoErrorInfoDetail("The resulting uncompressed size doesn't match the expected uncompressed size."));
    }

    destination.swap(uncompressed);
}

void RoZlib::UncompressInline(RoCharArray& buffer, const size_t compressedSize)
{
    const size_t uncompressedSize = buffer.size();
    RoCharArray uncompressed(uncompressedSize, 0);
    uLong resultingUncompressedSize = static_cast<uLong>(uncompressedSize);
    int result = uncompress(
        (Bytef*)&uncompressed[0],
        &resultingUncompressedSize,
        (const Bytef*)&buffer[0],
        static_cast<uLong>(compressedSize));

    if (Z_OK != result) {
        roTHROW(RoException("ZlibException")
                << GetZlibErrorDetail(result));
    } else if (uncompressedSize != resultingUncompressedSize) {
        roTHROW(RoException("ZlibException")
                << RoErrorInfoDetail("The resulting uncompressed size doesn't match the expected uncompressed size."));
    }
#if roPLATFORM_IS_WIN32
    memcpy_s(&buffer[0], buffer.size(), &uncompressed[0], uncompressedSize);
#else
    memcpy(&buffer[0], &uncompressed[0], uncompressedSize);
#endif // roPLATFORM_IS_WIN32
}


RoZlib::RoZlib(void)
{
}


RoZlib::~RoZlib(void)
{
}
