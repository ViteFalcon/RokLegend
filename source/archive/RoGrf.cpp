#include "RoGrf.h"
#include "RoGrfErrorInfo.h"
#include "RoGrfHeader.h"
#include "RoZlib.h"

#include <core/RoException.h>
#include <core/RoFileSystem.h>
#include <core/RoLog.h>
#include <core/RoMemoryDataStream.h>
#include <core/RoStringUtil.h>

#include <iterator>
#include <list>
#include <sstream>
#include <zlib.h>
#include <iostream>

/* DEFINEs to point to various tables */
#define DES_IP          tables0x40[0]           /** Initial Permutation (IP) */
#define DES_IP_INV      tables0x40[1]           /** Final Permutation (IP^-1) */
#define DES_S(num)      tables0x40[2 + num]     /** Selection functions (S) */
#define DES_PC2         tables0x30[0]           /** Permuted-choice 2 (PC-2) */
#define DES_E           tables0x30[1]           /** Bit-selection (E) */
#define DES_P           tables0x20[0]           /** (P) */
#define DES_PC1_1       tables0x1C[0]           /** Permuted-choice 1 (PC-1) part 1 */
#define DES_PC1_2       tables0x1C[1]           /** Permuted-choice 1 (PC-1) part 2 */
#define DES_LSHIFTS     tables0x10[0]           /** Left shifts per iteration */

namespace RoCrypt {
    /* Some DES tables */
    static const uint8 tables0x40[][0x40] = {
        /* Initial Permutation (IP) */
        {
            0x3A, 0x32, 0x2A, 0x22, 0x1A, 0x12, 0x0A, 0x02,
            0x3C, 0x34, 0x2C, 0x24, 0x1C, 0x14, 0x0C, 0x04,
            0x3E, 0x36, 0x2E, 0x26, 0x1E, 0x16, 0x0E, 0x06,
            0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08,
            0x39, 0x31, 0x29, 0x21, 0x19, 0x11, 0x09, 0x01,
            0x3B, 0x33, 0x2B, 0x23, 0x1B, 0x13, 0x0B, 0x03,
            0x3D, 0x35, 0x2D, 0x25, 0x1D, 0x15, 0x0D, 0x05,
            0x3F, 0x37, 0x2F, 0x27, 0x1F, 0x17, 0x0F, 0x07
        },
        /* Inverse Initial Permutation (IP^-1) */
        {
            0x28, 0x08, 0x30, 0x10, 0x38, 0x18, 0x40, 0x20,
            0x27, 0x07, 0x2F, 0x0F, 0x37, 0x17, 0x3F, 0x1F,
            0x26, 0x06, 0x2E, 0x0E, 0x36, 0x16, 0x3E, 0x1E,
            0x25, 0x05, 0x2D, 0x0D, 0x35, 0x15, 0x3D, 0x1D,
            0x24, 0x04, 0x2C, 0x0C, 0x34, 0x14, 0x3C, 0x1C,
            0x23, 0x03, 0x2B, 0x0B, 0x33, 0x13, 0x3B, 0x1B,
            0x22, 0x02, 0x2A, 0x0A, 0x32, 0x12, 0x3A, 0x1A,
            0x21, 0x01, 0x29, 0x09, 0x31, 0x11, 0x39, 0x19
        },
        /* 8 Selection functions (S) */
        {
            0x0E, 0x00, 0x04, 0x0F, 0x0D, 0x07, 0x01, 0x04,
            0x02, 0x0E, 0x0F, 0x02, 0x0B, 0x0D, 0x08, 0x01,
            0x03, 0x0A, 0x0A, 0x06, 0x06, 0x0C, 0x0C, 0x0B,
            0x05, 0x09, 0x09, 0x05, 0x00, 0x03, 0x07, 0x08,
            0x04, 0x0F, 0x01, 0x0C, 0x0E, 0x08, 0x08, 0x02,
            0x0D, 0x04, 0x06, 0x09, 0x02, 0x01, 0x0B, 0x07,
            0x0F, 0x05, 0x0C, 0x0B, 0x09, 0x03, 0x07, 0x0E,
            0x03, 0x0A, 0x0A, 0x00, 0x05, 0x06, 0x00, 0x0D
        }, {
            0x0F, 0x03, 0x01, 0x0D, 0x08, 0x04, 0x0E, 0x07,
            0x06, 0x0F, 0x0B, 0x02, 0x03, 0x08, 0x04, 0x0E,
            0x09, 0x0C, 0x07, 0x00, 0x02, 0x01, 0x0D, 0x0A,
            0x0C, 0x06, 0x00, 0x09, 0x05, 0x0B, 0x0A, 0x05,
            0x00, 0x0D, 0x0E, 0x08, 0x07, 0x0A, 0x0B, 0x01,
            0x0A, 0x03, 0x04, 0x0F, 0x0D, 0x04, 0x01, 0x02,
            0x05, 0x0B, 0x08, 0x06, 0x0C, 0x07, 0x06, 0x0C,
            0x09, 0x00, 0x03, 0x05, 0x02, 0x0E, 0x0F, 0x09
        }, {
            0x0A, 0x0D, 0x00, 0x07, 0x09, 0x00, 0x0E, 0x09,
            0x06, 0x03, 0x03, 0x04, 0x0F, 0x06, 0x05, 0x0A,
            0x01, 0x02, 0x0D, 0x08, 0x0C, 0x05, 0x07, 0x0E,
            0x0B, 0x0C, 0x04, 0x0B, 0x02, 0x0F, 0x08, 0x01,
            0x0D, 0x01, 0x06, 0x0A, 0x04, 0x0D, 0x09, 0x00,
            0x08, 0x06, 0x0F, 0x09, 0x03, 0x08, 0x00, 0x07,
            0x0B, 0x04, 0x01, 0x0F, 0x02, 0x0E, 0x0C, 0x03,
            0x05, 0x0B, 0x0A, 0x05, 0x0E, 0x02, 0x07, 0x0C
        }, {
            0x07, 0x0D, 0x0D, 0x08, 0x0E, 0x0B, 0x03, 0x05,
            0x00, 0x06, 0x06, 0x0F, 0x09, 0x00, 0x0A, 0x03,
            0x01, 0x04, 0x02, 0x07, 0x08, 0x02, 0x05, 0x0C,
            0x0B, 0x01, 0x0C, 0x0A, 0x04, 0x0E, 0x0F, 0x09,
            0x0A, 0x03, 0x06, 0x0F, 0x09, 0x00, 0x00, 0x06,
            0x0C, 0x0A, 0x0B, 0x01, 0x07, 0x0D, 0x0D, 0x08,
            0x0F, 0x09, 0x01, 0x04, 0x03, 0x05, 0x0E, 0x0B,
            0x05, 0x0C, 0x02, 0x07, 0x08, 0x02, 0x04, 0x0E
        }, {
            0x02, 0x0E, 0x0C, 0x0B, 0x04, 0x02, 0x01, 0x0C,
            0x07, 0x04, 0x0A, 0x07, 0x0B, 0x0D, 0x06, 0x01,
            0x08, 0x05, 0x05, 0x00, 0x03, 0x0F, 0x0F, 0x0A,
            0x0D, 0x03, 0x00, 0x09, 0x0E, 0x08, 0x09, 0x06,
            0x04, 0x0B, 0x02, 0x08, 0x01, 0x0C, 0x0B, 0x07,
            0x0A, 0x01, 0x0D, 0x0E, 0x07, 0x02, 0x08, 0x0D,
            0x0F, 0x06, 0x09, 0x0F, 0x0C, 0x00, 0x05, 0x09,
            0x06, 0x0A, 0x03, 0x04, 0x00, 0x05, 0x0E, 0x03
        }, {
            0x0C, 0x0A, 0x01, 0x0F, 0x0A, 0x04, 0x0F, 0x02,
            0x09, 0x07, 0x02, 0x0C, 0x06, 0x09, 0x08, 0x05,
            0x00, 0x06, 0x0D, 0x01, 0x03, 0x0D, 0x04, 0x0E,
            0x0E, 0x00, 0x07, 0x0B, 0x05, 0x03, 0x0B, 0x08,
            0x09, 0x04, 0x0E, 0x03, 0x0F, 0x02, 0x05, 0x0C,
            0x02, 0x09, 0x08, 0x05, 0x0C, 0x0F, 0x03, 0x0A,
            0x07, 0x0B, 0x00, 0x0E, 0x04, 0x01, 0x0A, 0x07,
            0x01, 0x06, 0x0D, 0x00, 0x0B, 0x08, 0x06, 0x0D
        }, {
            0x04, 0x0D, 0x0B, 0x00, 0x02, 0x0B, 0x0E, 0x07,
            0x0F, 0x04, 0x00, 0x09, 0x08, 0x01, 0x0D, 0x0A,
            0x03, 0x0E, 0x0C, 0x03, 0x09, 0x05, 0x07, 0x0C,
            0x05, 0x02, 0x0A, 0x0F, 0x06, 0x08, 0x01, 0x06,
            0x01, 0x06, 0x04, 0x0B, 0x0B, 0x0D, 0x0D, 0x08,
            0x0C, 0x01, 0x03, 0x04, 0x07, 0x0A, 0x0E, 0x07,
            0x0A, 0x09, 0x0F, 0x05, 0x06, 0x00, 0x08, 0x0F,
            0x00, 0x0E, 0x05, 0x02, 0x09, 0x03, 0x02, 0x0C
        }, {
            0x0D, 0x01, 0x02, 0x0F, 0x08, 0x0D, 0x04, 0x08,
            0x06, 0x0A, 0x0F, 0x03, 0x0B, 0x07, 0x01, 0x04,
            0x0A, 0x0C, 0x09, 0x05, 0x03, 0x06, 0x0E, 0x0B,
            0x05, 0x00, 0x00, 0x0E, 0x0C, 0x09, 0x07, 0x02,
            0x07, 0x02, 0x0B, 0x01, 0x04, 0x0E, 0x01, 0x07,
            0x09, 0x04, 0x0C, 0x0A, 0x0E, 0x08, 0x02, 0x0D,
            0x00, 0x0F, 0x06, 0x0C, 0x0A, 0x09, 0x0D, 0x00,
            0x0F, 0x03, 0x03, 0x05, 0x05, 0x06, 0x08, 0x0B
        }
    };

    static const uint8_t tables0x30[][0x30] = {
        /* Permuted Choice 2 (PC-2) */
        {
            0x0E, 0x11, 0x0B, 0x18, 0x01, 0x05, 0x03, 0x1C,
            0x0F, 0x06, 0x15, 0x0A, 0x17, 0x13, 0x0C, 0x04,
            0x1A, 0x08, 0x10, 0x07, 0x1B, 0x14, 0x0D, 0x02,
            0x29, 0x34, 0x1F, 0x25, 0x2F, 0x37, 0x1E, 0x28,
            0x33, 0x2D, 0x21, 0x30, 0x2C, 0x31, 0x27, 0x38,
            0x22, 0x35, 0x2E, 0x2A, 0x32, 0x24, 0x1D, 0x20
        },
        /* Bit-selection table (E) */
        {
            0x20, 0x01, 0x02, 0x03, 0x04, 0x05,
            0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
            0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
            0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
            0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
            0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x01
        }
    };

    static const uint8_t tables0x20[][0x20] = {
        /* P */
        {
            0x10, 0x07, 0x14, 0x15,
            0x1D, 0x0C, 0x1C, 0x11,
            0x01, 0x0F, 0x17, 0x1A,
            0x05, 0x12, 0x1F, 0x0A,
            0x02, 0x08, 0x18, 0x0E,
            0x20, 0x1B, 0x03, 0x09,
            0x13, 0x0D, 0x1E, 0x06,
            0x16, 0x0B, 0x04, 0x19
        }
    };

    static const uint8_t tables0x1C[][0x1C] = {
        {
            0x39, 0x31, 0x29, 0x21, 0x19, 0x11, 0x09, 0x01,
            0x3A, 0x32, 0x2A, 0x22, 0x1A, 0x12, 0x0A, 0x02,
            0x3B, 0x33, 0x2B, 0x23, 0x1B, 0x13, 0x0B, 0x03,
            0x3C, 0x34, 0x2C, 0x24
        },
        {
            0x3F, 0x37, 0x2F, 0x27, 0x1F, 0x17, 0x0F, 0x07,
            0x3E, 0x36, 0x2E, 0x26, 0x1E, 0x16, 0x0E, 0x06,
            0x3D, 0x35, 0x2D, 0x25, 0x1D, 0x15, 0x0D, 0x05,
            0x1C, 0x14, 0x0C, 0x04
        }
    };

    static const uint8_t tables0x10[][0x10] = {
        /*! Left shifts each iteration */
        {
            0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01
        }
    };

    /*! Used to map bit number to bit */
    static const uint8_t BitMap[0x08] = {
        0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1
    };

    uint8 BitMaskTable[8] = {
        0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
    };

    uint8 BitSwapTable1[64] = {
        58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
    };

    uint8 BitSwapTable2[64] = {
        40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
    };

    uint8 BitSwapTable3[32] = {
        16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
        2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
    };

    uint8 NibbleData[4][64] = {
        {
            0xef, 0x03, 0x41, 0xfd, 0xd8, 0x74, 0x1e, 0x47, 0x26, 0xef, 0xfb, 0x22, 0xb3, 0xd8, 0x84, 0x1e,
            0x39, 0xac, 0xa7, 0x60, 0x62, 0xc1, 0xcd, 0xba, 0x5c, 0x96, 0x90, 0x59, 0x05, 0x3b, 0x7a, 0x85,
            0x40, 0xfd, 0x1e, 0xc8, 0xe7, 0x8a, 0x8b, 0x21, 0xda, 0x43, 0x64, 0x9f, 0x2d, 0x14, 0xb1, 0x72,
            0xf5, 0x5b, 0xc8, 0xb6, 0x9c, 0x37, 0x76, 0xec, 0x39, 0xa0, 0xa3, 0x05, 0x52, 0x6e, 0x0f, 0xd9,
        },
        {
            0xa7, 0xdd, 0x0d, 0x78, 0x9e, 0x0b, 0xe3, 0x95, 0x60, 0x36, 0x36, 0x4f, 0xf9, 0x60, 0x5a, 0xa3,
            0x11, 0x24, 0xd2, 0x87, 0xc8, 0x52, 0x75, 0xec, 0xbb, 0xc1, 0x4c, 0xba, 0x24, 0xfe, 0x8f, 0x19,
            0xda, 0x13, 0x66, 0xaf, 0x49, 0xd0, 0x90, 0x06, 0x8c, 0x6a, 0xfb, 0x91, 0x37, 0x8d, 0x0d, 0x78,
            0xbf, 0x49, 0x11, 0xf4, 0x23, 0xe5, 0xce, 0x3b, 0x55, 0xbc, 0xa2, 0x57, 0xe8, 0x22, 0x74, 0xce,
        },
        {
            0x2c, 0xea, 0xc1, 0xbf, 0x4a, 0x24, 0x1f, 0xc2, 0x79, 0x47, 0xa2, 0x7c, 0xb6, 0xd9, 0x68, 0x15,
            0x80, 0x56, 0x5d, 0x01, 0x33, 0xfd, 0xf4, 0xae, 0xde, 0x30, 0x07, 0x9b, 0xe5, 0x83, 0x9b, 0x68,
            0x49, 0xb4, 0x2e, 0x83, 0x1f, 0xc2, 0xb5, 0x7c, 0xa2, 0x19, 0xd8, 0xe5, 0x7c, 0x2f, 0x83, 0xda,
            0xf7, 0x6b, 0x90, 0xfe, 0xc4, 0x01, 0x5a, 0x97, 0x61, 0xa6, 0x3d, 0x40, 0x0b, 0x58, 0xe6, 0x3d,
        },
        {
            0x4d, 0xd1, 0xb2, 0x0f, 0x28, 0xbd, 0xe4, 0x78, 0xf6, 0x4a, 0x0f, 0x93, 0x8b, 0x17, 0xd1, 0xa4,
            0x3a, 0xec, 0xc9, 0x35, 0x93, 0x56, 0x7e, 0xcb, 0x55, 0x20, 0xa0, 0xfe, 0x6c, 0x89, 0x17, 0x62,
            0x17, 0x62, 0x4b, 0xb1, 0xb4, 0xde, 0xd1, 0x87, 0xc9, 0x14, 0x3c, 0x4a, 0x7e, 0xa8, 0xe2, 0x7d,
            0xa0, 0x9f, 0xf6, 0x5c, 0x6a, 0x09, 0x8d, 0xf0, 0x0f, 0xe3, 0x53, 0x25, 0x95, 0x36, 0x28, 0xcb,
        }
    };

    roINLINE void NibbleSwap(char *src, uint32 len)
    {
        for (; len != 0; len--, src++)
            *src = (*src >> 4) | (*src << 4);
    }

    roINLINE void BitConvert(char *src, uint8 *BitSwapTable)
    {
        char tmp[8];

        memset(tmp, 0, 8);
        for (uint8 i = 0, j; i != 64; i++) {
            j = BitSwapTable[i] - 1;

            if (src[(j >> 3) & 7] & BitMaskTable[j & 7])
                tmp[(i >> 3) & 7] |= BitMaskTable[i & 7];
        }

        memcpy(src, tmp, 8);
    }

    roINLINE uint32& to32BitReference(uint8* src)
    {
        return *(uint32*) src;
    }

    roINLINE void BitConvert4(uint8 *src)
    {
        uint8 tmp[8];

        tmp[0] = ((src[7] << 5) | (src[4] >> 3)) & 0x3f;    // ..0 vutsr
        tmp[1] = ((src[4] << 1) | (src[5] >> 7)) & 0x3f;    // ..srqpo n
        tmp[2] = ((src[4] << 5) | (src[5] >> 3)) & 0x3f;    // ..o nmlkj
        tmp[3] = ((src[5] << 1) | (src[6] >> 7)) & 0x3f;    // ..kjihg f
        tmp[4] = ((src[5] << 5) | (src[6] >> 3)) & 0x3f;    // ..g fedcb
        tmp[5] = ((src[6] << 1) | (src[7] >> 7)) & 0x3f;    // ..cba98 7
        tmp[6] = ((src[6] << 5) | (src[7] >> 3)) & 0x3f;    // ..8 76543
        tmp[7] = ((src[7] << 1) | (src[4] >> 7)) & 0x3f;    // ..43210 v

        for (uint8 i = 0; i != 4; i++)
            tmp[i] = (NibbleData[i][tmp[i * 2]] & 0xf0) | (NibbleData[i][tmp[i * 2 + 1]] & 0x0f);

        to32BitReference(tmp + 4) = 0;
        for (uint8 i = 0, j; i != 32; i++) {
            j = BitSwapTable3[i] - 1;

            if (tmp[j >> 3] & BitMaskTable[j & 7])
                tmp[(i >> 3) + 4] |= BitMaskTable[i & 7];
        }

        to32BitReference(src) ^= to32BitReference(tmp + 4);
    }

    RoCharArray CreateDesKey(const std::string& file)
    {
        RoCharArray key{};
        key.resize(0x80, '\0');
#if roGRF_FIXED_KEY
#endif
        return key;
    }

    void DesPermute(uint8* block, const uint8* table)
    {
        uint8 tempBlock[8];
        uint32 temp;
        memset(tempBlock, 0, 8);
        for (uint8 i = 0; i < 0x40; ++i) {
            temp = table[i] - 1;
            if (block[temp >> 3] & BitMap[temp & 7]) {
                tempBlock[i >> 3] |= BitMap[i & 7];
            }
        }
        memcpy(block, tempBlock, 8);
    }

    void DesProcessRawBlock(uint8* block, const uint8* key)
    {
        uint8 tempBlock[2][8];
        uint32 temp;
        memset(tempBlock[0], 0, 8);
        for (uint8 i = 0; i < 0x30; ++i) {
            temp = DES_E[i] + 0x1F;
            if (block[temp >> 3] & BitMap[temp & 7]) {
                tempBlock[0][i / 6] |= BitMap[i % 6];
            }
        }
        for (uint8 i = 0; i < 8; ++i) {
            tempBlock[0][i] ^= key[i];
        }
        memset(tempBlock[1], 0, 8);
        for (uint8 i = 0; i < 8; ++i) {
            if (i % 2) {
                tempBlock[1][i >> 1] += DES_S(i)[tempBlock[0][i] >> 2];
            } else {
                tempBlock[1][i >> 1] = DES_S(i)[tempBlock[0][i] >> 2] << 4;
            }
        }
        memset(tempBlock[0], 0, 8);
        for (uint8 i = 0; i < 0x20; ++i) {
            temp = DES_P[i] - 1;
            if (tempBlock[1][temp >> 3] & BitMap[temp & 7]) {
                tempBlock[0][i >> 3] |= BitMap[i & 7];
            }
        }
        block[0] ^= tempBlock[0][0];
        block[1] ^= tempBlock[0][1];
        block[2] ^= tempBlock[0][2];
        block[3] ^= tempBlock[0][3];
    }

    void DesDecryptBlock(uint8 rounds, uint8* dst, const uint8* src, const char* key)
    {
        uint8 temp[4];
        memcpy(dst, src, 8);
        DesPermute(dst, DES_IP);
        if (rounds > 0) {
            for (uint8 i = 0; i < rounds; ++i) {
                int j = 0xF - i;
                DesProcessRawBlock(dst, (const uint8*) (key + j * 8));
                // Swap 4 blocks from L/R
                memcpy(temp, dst, 4);
                memcpy(dst, dst + 4, 4);
                memcpy(dst + 4, temp, 4);
            }
        }
        // Swap L/R 4 byte blocks again
        memcpy(temp, dst, 4);
        memcpy(dst, dst + 4, 4);
        memcpy(dst + 4, temp, 4);
        // Run final permutation
        DesPermute(dst, DES_IP_INV);
    }

    void DesDecrypt(char* dst, const char* src, uint32 length, const char* key)
    {
        uint8 cycle = 0;
        for (size_t i = 0; i < length / 8; dst += 8, src += 8) {
            DesDecryptBlock(1, (uint8*) src, (const uint8*) dst, &key[0]);
        }
    }

#pragma warning(push)
#pragma warning(disable:4309)
    void MixedDecrypt(char* dst, const char* src, uint32 len, uint32 cycle, const char* key)
    {
        uint32_t i;
        uint8_t j, tmp;
        char *orig;

        orig = dst;

        /* Modify the cycle */
        if (cycle < 3) {
            cycle = 1;
        } else if (cycle < 5) {
            cycle++;
        } else if (cycle < 7) {
            cycle += 9;
        } else {
            cycle += 0xF;
        }

        for (i = j = 0; i < (len / 8); i++, dst += 8, src += 8) {
            /* Check if its one of the first 0x14, or if its evenly
            * divisible by cycle
            */
            if (i < 0x14 || !(i%cycle)) {
                DesDecryptBlock(1, (uint8_t *) dst, (const uint8_t *) src, key);
            } else {
                /* Check if its time to modify byte order */
                if (j == 7) {
                    /* Swap around some bytes */
                    // 3450162
                    memcpy(dst, src + 3, 2);
                    // 01_____
                    dst[2] = src[6];
                    // 012____
                    memcpy(dst + 3, src, 3);
                    // 012345_
                    dst[6] = src[5];
                    // 0123456

                    /* Modify byte 7 */
                    if ((tmp = src[7]) <= 0x77) {
                        if (tmp == 0x77) {
                            /* 0x77 */
                            dst[7] = 0x48;
                        } else if (!tmp) {
                            /* 0x00 */
                            dst[7] = 0x2B;
                        } else if (!(--tmp)) {
                            /* 0x01 */
                            dst[7] = 0x68;
                        } else if (!(tmp -= 0x2A)) {
                            /* 0x2B */
                            dst[7] = 0x00;
                        } else if (!(tmp -= 0x1D)) {
                            /* 0x48 */
                            dst[7] = 0x77;
                        } else if (!(tmp -= 0x18)) {
                            /* 0x60 */
                            dst[7] = 0xFF;
                        } else if (!(tmp -= 0x08)) {
                            /* 0x68 */
                            dst[7] = 0x01;
                        } else if (!(tmp -= 0x04)) {
                            /* 0x6C */
                            dst[7] = 0x80;
                        } else {
                            dst[7] = src[7];
                        }
                    } else {
                        if (!(tmp -= 0x80)) {
                            /* 0x80 */
                            dst[7] = 0x6C;
                        } else if (!(tmp -= 0x39)) {
                            /* 0xB9 */
                            dst[7] = 0xC0;
                        } else if (!(tmp -= 0x07)) {
                            /* 0xC0 */
                            dst[7] = 0xB9;
                        } else if (!(tmp -= 0x2B)) {
                            /* 0xEB */
                            dst[7] = 0xFE;
                        } else if (!(tmp -= 0x13)) {
                            /* 0xFE */
                            dst[7] = 0xEB;
                        } else if (!(--tmp)) {
                            /* 0xFF */
                            dst[7] = 0x60;
                        } else {
                            dst[7] = src[7];
                        }
                    }
                    j = 0;
                } else {
                    memcpy(dst, src, 8);
                }
                j++;
            }
        }
    }
#pragma warning(pop)

    void DecodeGrfFileContentBuffer(RoCharArray& buffer, const RoGrfFile& file)
    {
        RoCharArray key = CreateDesKey(file.getName().asUTF8());
        const uint32 length = file.getEncryptedSize();
        RoCharArray result(length, '\0');

        if (file.isDesEncrypted()) {
            char* src = &buffer[0];
            char* dst = &result[0];
            uint32 i = length / 8;
            if (i > 0x14) {
                i = 0x14;
                memcpy(dst + 0x14 * 8, src + 0x14 * 8, length - 0x14 * 8);
            }
            DesDecrypt(dst, src, i * 8, &key[0]);

        } else if (file.isMixedEncrypted()) {
            uint32 cycle = 0;
            for (uint32 i = file.getCompressedSize(); i > 0; i /= 10, ++cycle);
            if (cycle < 1) {
                cycle = 1;
            }
            MixedDecrypt(&result[0], &buffer[0], length, cycle, &key[0]);
        }
        buffer.swap(result);
    }
}

RoGrf::RoGrf(size_t grfId, const RoGrfStorageBackendPtr& store, const RoGrfHeader& header, const RoString& fileName)
: mGrfId(grfId)
, mStore(store)
, mHeader(header)
, mFileSize(RoFileSystem::GetFileSize(fileName.asWStr()))
, mLastUpdateTime(RoFileSystem::GetLastUpdatedTime(fileName.asWStr()))
, mContentSize(mFileSize - header.getOffset())
, mFileName(fileName)
#if roPLATFORM_IS_WIN32
, mFileStream(fileName.c_str(), std::ios::in | std::ios::binary)
#else
, mFileStream(fileName.asUTF8_c_str(), std::ios::in | std::ios::binary)
#endif
{
    mFileStream.seekg(header.getOffset());
}
//------------------------------------------------------------------------------
RoGrf::~RoGrf()
{
}
//------------------------------------------------------------------------------
RoDataStreamPtr RoGrf::getFileContentsOf(const RoString& fileName)
{
    const RoGrfFilePtr file = mStore->getFileInfo(fileName);
    const size_t fileContentSize = file->getOriginalSize();
    RoCharArray fileContents(fileContentSize, '\0');

    // Open a separate stream to the file.
    {
        const std::string fileName = mFileName.asUTF8();
        std::ifstream grfStream(fileName.c_str(), std::ios::in);
        grfStream.seekg(file->getOffset());
        grfStream.read(&fileContents[0], file->getEncryptedSize());
    }

    if (file->isEncrypted()) {
        RoCrypt::DecodeGrfFileContentBuffer(fileContents, *file);
    }


    try {
        const size_t originalSize = file->getOriginalSize();
        RoCharArray actualFileContents{};
        actualFileContents.resize(originalSize, '\0');
        RoZlib::Uncompress(actualFileContents, originalSize, fileContents);
        fileContents.swap(actualFileContents);
    } catch (RoException& exception) {
        exception << RoErrorInfoFileName(fileName);
        const char* errorMessage = exception.what();
        roLOG_CRIT << errorMessage;
        roTHROW(exception);
    } catch (...) {
        throw RoInvalidGrfException()
            << RoErrorInfoFileName(fileName)
            << RoErrorInfoDetail("Failed to uncompress file contents.");
    }

    RoDataStreamPtr dataStream = std::make_shared<RoMemoryDataStream>(fileContentSize);
    dataStream->write(&fileContents[0], fileContentSize);
    return dataStream;
}
//------------------------------------------------------------------------------
struct RoGrfCachedFileInfo {
    std::time_t lastUpdatedTime;
    std::size_t size;
    RoString cacheFilePath;
};
//------------------------------------------------------------------------------
RoGrf::OptionalDataStreamPtr RoGrf::getCachedFileContents(const RoString& fileName)
{
    RoPath cacheFilePath = getCachedFileName(fileName);
    if (!RoFileSystem::FileExists(cacheFilePath)) {
        return OptionalDataStreamPtr();
    }
    RoDataStreamPtr dataStream;
    return OptionalDataStreamPtr(dataStream);
}
//------------------------------------------------------------------------------
RoPath RoGrf::getCachedFileName(const RoString& fileName)
{
    static optional<RoPath> CACHE_FILE_DIR;
    if (!CACHE_FILE_DIR.is_initialized()) {
        const RoPath grfFilePath = RoPath(mFileName.asWStr());
        const RoPath cacheSubDir = grfFilePath.leaf().wstring() + L".cache";
        CACHE_FILE_DIR.reset(RoFileSystem::GetGameDirectory() / cacheSubDir);
    }
    return CACHE_FILE_DIR.get() / RoPath(fileName);
}
//------------------------------------------------------------------------------
RoString RoGrf::decodeFileName(const RoCharArray& encodedFileName)
{
    std::string result;
    result.assign(encodedFileName.begin(), encodedFileName.end());

    char* p = &result[0];
    uint8 encryptionLength = (uint8) encodedFileName.size();
    for (uint8 i = 0; i < encryptionLength; i += 8) {
        RoCrypt::NibbleSwap(p + i, 8);
        RoCrypt::BitConvert(p + i, RoCrypt::BitSwapTable1);
        RoCrypt::BitConvert4((uint8*)(p + i));
        RoCrypt::BitConvert(p + i, RoCrypt::BitSwapTable2);
    }
    return RoString(result);
}
//------------------------------------------------------------------------------
bool RoGrf::hasSpecialExtension(const RoString& file)
{
    /** Special file extensions.
     *
     * Files with these extentions are handled differently in GPFs.
     */
    static const size_t specialExtensionsCount = 4;
    static const RoString specialExts[specialExtensionsCount] = {
        ".gnd",
        ".gat",
        ".act",
        ".str"
    };
    const RoString fileExtension = RoStringUtil::ToLower(file.substr(file.length() - 4));
    for (int i = 0; i < specialExtensionsCount; ++i) {
        if (fileExtension == specialExts[i]) {
            return true;
        }
    }
    return false;
}

extern "C" {
#   include <grf.h>
}
#include <core/RoScopedPtr.h>

RoGrf2::RoGrf2(const RoString& name) : mHandle(nullptr)
{
    const std::string utf8Name = name.asUTF8();
    mHandle = grf_load(utf8Name.c_str(), false);
    if (mHandle == nullptr) {
        roTHROW(RoException() << RoErrorInfoDetail("Failed to load GRF!"));
    }
}

RoGrf2::~RoGrf2()
{
    grf_free(static_cast<grf_handle>(mHandle));
}

RoDataStreamPtr RoGrf2::getFileContentsOf(const RoString& fileName)
{
    const std::string utf8Name = fileName.asUTF8();
    grf_node_handle fileHandle = grf_get_file(static_cast<grf_handle>(mHandle), utf8Name.c_str());
    uint32_t fileSize = grf_file_get_size(fileHandle);
    char *buffer = new char[fileSize];
    uint32_t bytesRead = grf_file_get_contents(fileHandle, buffer);
    roLOG_INFO << "Expected " << fileSize << " bytes. Read " << bytesRead << " bytes.";
    return std::make_shared<RoMemoryDataStream>(buffer, fileSize, true, RoMemoryDataStream::AccessMode::READ);
}
