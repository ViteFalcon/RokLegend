#pragma once
#ifndef ROKLEGEND_ZLIB_H
#define ROKLEGEND_ZLIB_H

#include <Core/RoPrerequisites.h>

class RoZlib
{
public:
    static void Uncompress(RoCharArray& destination, const size_t uncompressedSize, const RoCharArray& source);
    /**
     * Uncompresses the given buffer (inline)
     *
     * @param buffer
     *            The buffer which contains the compressed data. The assumption is that this buffer
     *            has enough space to store the uncompressed data.
     * @param compressedSize
     *            The size of the compressed data within the buffer
     */
    static void UncompressInline(RoCharArray& buffer, const size_t compressedSize);

private:
    RoZlib(void);
    ~RoZlib(void);
};

#endif // ROKLEGEND_ZLIB_H
