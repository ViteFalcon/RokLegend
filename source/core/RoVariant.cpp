////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoVariant.h"
#include "RoStringUtil.h"

#include <algorithm>
#include <locale>
#include <sstream>
#include <vector>

#ifdef _MSC_VER
#    pragma warning(disable: 4800)
#endif // _MSC_VER

/** Helper Macros **/
#define POD_CONVERSION_FUNC_NAME(FromType, ToType) Detail::VariantConvert<FromType, ToType>
#define POD_CONVERT_TO_STR_FUNC_NAME(FromType) Detail::VariantConvertToStr<FromType>
#define CONVERT_TO_ROSTRING_FUNC_NAME(FromType) Detail::VariantConvertToRoString<FromType>
#define POD_CONVERT_TO_WSTR_FUNC_NAME(FromType) Detail::VariantConvertToWStr<FromType>

/** Registration Helper Macros **/
#define POD_CONVERSION_FUNC_REGISTER(FromType, ToType) (typeid(FromType), typeid(ToType), POD_CONVERSION_FUNC_NAME(FromType, ToType))
#define POD_CONVERSION_FUNC_REGISTER2(FromType, ToType) POD_CONVERSION_FUNC_REGISTER(FromType, ToType)\
    POD_CONVERSION_FUNC_REGISTER(FromType, unsigned ToType)

#define POD_CONVERT_TO_STR_FUNC_REGISTER(FromType) (typeid(FromType), typeid(std::string), POD_CONVERT_TO_STR_FUNC_NAME(FromType))
#define CONVERT_TO_ROSTRING_FUNC_REGISTER(FromType) (typeid(FromType), typeid(RoString), CONVERT_TO_ROSTRING_FUNC_NAME(FromType))
#define POD_CONVERT_TO_WSTR_FUNC_REGISTER(FromType) (typeid(FromType), typeid(std::wstring), POD_CONVERT_TO_WSTR_FUNC_NAME(FromType))

#define POD_CONVERSION_FUNC_REGISTER_ALL_POD(FromType) POD_CONVERSION_FUNC_REGISTER(FromType, bool)\
    POD_CONVERSION_FUNC_REGISTER2(FromType, char)\
    POD_CONVERSION_FUNC_REGISTER(FromType, int8)\
    POD_CONVERSION_FUNC_REGISTER(FromType, uint8)\
    POD_CONVERSION_FUNC_REGISTER2(FromType, short)\
    POD_CONVERSION_FUNC_REGISTER(FromType, int16)\
    POD_CONVERSION_FUNC_REGISTER(FromType, uint16)\
    POD_CONVERSION_FUNC_REGISTER2(FromType, int)\
    POD_CONVERSION_FUNC_REGISTER(FromType, int32)\
    POD_CONVERSION_FUNC_REGISTER(FromType, uint32)\
    POD_CONVERSION_FUNC_REGISTER2(FromType, long)\
    POD_CONVERSION_FUNC_REGISTER2(FromType, long long)\
    POD_CONVERSION_FUNC_REGISTER(FromType, int64)\
    POD_CONVERSION_FUNC_REGISTER(FromType, uint64)\
    POD_CONVERSION_FUNC_REGISTER(FromType, float)\
    POD_CONVERSION_FUNC_REGISTER(FromType, double)\
    POD_CONVERT_TO_STR_FUNC_REGISTER(FromType)\
    CONVERT_TO_ROSTRING_FUNC_REGISTER(FromType)\
    POD_CONVERT_TO_WSTR_FUNC_REGISTER(FromType)

#define POD_CONVERSION_FUNC_REGISTER_ALL_POD2(FromType) POD_CONVERSION_FUNC_REGISTER_ALL_POD(FromType)\
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(unsigned FromType)

// UTF-8 to UTF-16 conversion code
/*
 * Copyright 2001-2004 Unicode, Inc.
 *
 * Disclaimer
 *
 * This source code is provided as is by Unicode, Inc. No claims are
 * made as to fitness for any particular purpose. No warranties of any
 * kind are expressed or implied. The recipient agrees to determine
 * applicability of information provided. If this file has been
 * purchased on magnetic or optical media from Unicode, Inc., the
 * sole remedy for any claim will be exchange of defective media
 * within 90 days of receipt.
 *
 * Limitations on Rights to Redistribute This Code
 *
 * Unicode, Inc. hereby grants the right to freely use the information
 * supplied in this file in the creation of products supporting the
 * Unicode Standard, and to make copies of this file in any form
 * for internal or external distribution as long as this notice
 * remains attached.
 */

/* ---------------------------------------------------------------------

    Conversions between UTF32, UTF-16, and UTF-8.  Header file.

    Several funtions are included here, forming a complete set of
    conversions between the three formats.  UTF-7 is not included
    here, but is handled in a separate source file.

    Each of these routines takes pointers to input buffers and output
    buffers.  The input buffers are const.

    Each routine converts the text between *sourceStart and sourceEnd,
    putting the result into the buffer between *targetStart and
    targetEnd. Note: the end pointers are *after* the last item: e.g.
    *(sourceEnd - 1) is the last item.

    The return result indicates whether the conversion was successful,
    and if not, whether the problem was in the source or target buffers.
    (Only the first encountered problem is indicated.)

    After the conversion, *sourceStart and *targetStart are both
    updated to point to the end of last text successfully converted in
    the respective buffers.

    Input parameters:
        sourceStart - pointer to a pointer to the source buffer.
                The contents of this are modified on return so that
                it points at the next thing to be converted.
        targetStart - similarly, pointer to pointer to the target buffer.
        sourceEnd, targetEnd - respectively pointers to the ends of the
                two buffers, for overflow checking only.

    These conversion functions take a ConversionFlags argument. When this
    flag is set to strict, both irregular sequences and isolated surrogates
    will cause an error.  When the flag is set to lenient, both irregular
    sequences and isolated surrogates are converted.

    Whether the flag is strict or lenient, all illegal sequences will cause
    an error return. This includes sequences such as: <F4 90 80 80>, <C0 80>,
    or <A0> in UTF-8, and values above 0x10FFFF in UTF-32. Conformant code
    must check for illegal sequences.

    When the flag is set to lenient, characters over 0x10FFFF are converted
    to the replacement character; otherwise (when the flag is set to strict)
    they constitute an error.

    Output parameters:
        The value "sourceIllegal" is returned from some routines if the input
        sequence is malformed.  When "sourceIllegal" is returned, the source
        value will point to the illegal value that caused the problem. E.g.,
        in UTF-8 when a sequence is malformed, it points to the start of the
        malformed sequence.

    Author: Mark E. Davis, 1994.
    Rev History: Rick McGowan, fixes & updates May 2001.
                 Fixes & updates, Sept 2001.

------------------------------------------------------------------------ */

/* ---------------------------------------------------------------------
    The following 4 definitions are compiler-specific.
    The C standard does not guarantee that wchar_t has at least
    16 bits, so wchar_t is no less portable than unsigned short!
    All should be unsigned values to avoid sign extension during
    bit mask & shift operations.
------------------------------------------------------------------------ */

typedef unsigned long   UTF32;  /* at least 32 bits */
typedef unsigned short  UTF16;  /* at least 16 bits */
typedef unsigned char   UTF8;   /* typically 8 bits */
typedef unsigned char   Boolean; /* 0 or 1 */

/* Some fundamental constants */
#define UNI_REPLACEMENT_CHAR (UTF32)0x0000FFFD
#define UNI_MAX_BMP (UTF32)0x0000FFFF
#define UNI_MAX_UTF16 (UTF32)0x0010FFFF
#define UNI_MAX_UTF32 (UTF32)0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32 (UTF32)0x0010FFFF

typedef enum {
        conversionOK,           /* conversion successful */
        sourceExhausted,        /* partial character in source, but hit end */
        targetExhausted,        /* insuff. room in target for conversion */
        sourceIllegal           /* source sequence is illegal/malformed */
} ConversionResult;

typedef enum {
        strictConversion = 0,
        lenientConversion
} ConversionFlags;

#ifdef CVTUTF_DEBUG
#include <stdio.h>
#endif

static const int halfShift  = 10; /* used for shifting by 10 bits */

static const UTF32 halfBase = 0x0010000UL;
static const UTF32 halfMask = 0x3FFUL;

#define UNI_SUR_HIGH_START  (UTF32)0xD800
#define UNI_SUR_HIGH_END    (UTF32)0xDBFF
#define UNI_SUR_LOW_START   (UTF32)0xDC00
#define UNI_SUR_LOW_END     (UTF32)0xDFFF
#define false      0
#define true        1

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF32toUTF16 (
        const UTF32** sourceStart, const UTF32* sourceEnd,
        UTF16** targetStart, UTF16* targetEnd, ConversionFlags flags) {
    ConversionResult result = conversionOK;
    const UTF32* source = *sourceStart;
    UTF16* target = *targetStart;
    while (source < sourceEnd) {
        UTF32 ch;
        if (target >= targetEnd) {
            result = targetExhausted; break;
        }
        ch = *source++;
        if (ch <= UNI_MAX_BMP) { /* Target is a character <= 0xFFFF */
            /* UTF-16 surrogate values are illegal in UTF-32; 0xffff or 0xfffe are both reserved values */
            if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
                if (flags == strictConversion) {
                    --source; /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                } else {
                    *target++ = UNI_REPLACEMENT_CHAR;
                }
            } else {
                *target++ = (UTF16)ch; /* normal case */
            }
        } else if (ch > UNI_MAX_LEGAL_UTF32) {
            if (flags == strictConversion) {
                result = sourceIllegal;
            } else {
                *target++ = UNI_REPLACEMENT_CHAR;
            }
        } else {
            /* target is a character in range 0xFFFF - 0x10FFFF. */
            if (target + 1 >= targetEnd) {
                --source; /* Back up source pointer! */
                result = targetExhausted; break;
            }
            ch -= halfBase;
            *target++ = (UTF16)((ch >> halfShift) + UNI_SUR_HIGH_START);
            *target++ = (UTF16)((ch & halfMask) + UNI_SUR_LOW_START);
        }
    }
    *sourceStart = source;
    *targetStart = target;
    return result;
}

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF16toUTF32 (
        const UTF16** sourceStart, const UTF16* sourceEnd,
        UTF32** targetStart, UTF32* targetEnd, ConversionFlags flags) {
    ConversionResult result = conversionOK;
    const UTF16* source = *sourceStart;
    UTF32* target = *targetStart;
    UTF32 ch, ch2;
    while (source < sourceEnd) {
        const UTF16* oldSource = source; /*  In case we have to back up because of target overflow. */
        ch = *source++;
        /* If we have a surrogate pair, convert to UTF32 first. */
        if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END) {
            /* If the 16 bits following the high surrogate are in the source buffer... */
            if (source < sourceEnd) {
                ch2 = *source;
                /* If it's a low surrogate, convert to UTF32. */
                if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END) {
                    ch = ((ch - UNI_SUR_HIGH_START) << halfShift)
                        + (ch2 - UNI_SUR_LOW_START) + halfBase;
                    ++source;
                } else if (flags == strictConversion) { /* it's an unpaired high surrogate */
                    --source; /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                }
            } else { /* We don't have the 16 bits following the high surrogate. */
                --source; /* return to the high surrogate */
                result = sourceExhausted;
                break;
            }
        } else if (flags == strictConversion) {
            /* UTF-16 surrogate values are illegal in UTF-32 */
            if (ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END) {
                --source; /* return to the illegal value itself */
                result = sourceIllegal;
                break;
            }
        }
        if (target >= targetEnd) {
            source = oldSource; /* Back up source pointer! */
            result = targetExhausted; break;
        }
        *target++ = ch;
    }
    *sourceStart = source;
    *targetStart = target;
#ifdef CVTUTF_DEBUG
if (result == sourceIllegal) {
    fprintf(stderr, "ConvertUTF16toUTF32 illegal seq 0x%04x,%04x\n", ch, ch2);
    fflush(stderr);
}
#endif
    return result;
}

/* --------------------------------------------------------------------- */

/*
 * Index into the table below with the first byte of a UTF-8 sequence to
 * get the number of trailing bytes that are supposed to follow it.
 * Note that *legal* UTF-8 values can't have 4 or 5-bytes. The table is
 * left as-is for anyone who may want to do such conversion, which was
 * allowed in earlier algorithms.
 */
static const char trailingBytesForUTF8[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

/*
 * Magic values subtracted from a buffer value during UTF8 conversion.
 * This table contains as many values as there might be trailing bytes
 * in a UTF-8 sequence.
 */
static const UTF32 offsetsFromUTF8[6] = { 0x00000000UL, 0x00003080UL, 0x000E2080UL,
                     0x03C82080UL, 0xFA082080UL, 0x82082080UL };

/*
 * Once the bits are split out into bytes of UTF-8, this is a mask OR-ed
 * into the first byte, depending on how many bytes follow.  There are
 * as many entries in this table as there are UTF-8 sequence types.
 * (I.e., one byte sequence, two byte... etc.). Remember that sequencs
 * for *legal* UTF-8 will be 4 or fewer bytes total.
 */
static const UTF8 firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

/* --------------------------------------------------------------------- */

/* The interface converts a whole buffer to avoid function-call overhead.
 * Constants have been gathered. Loops & conditionals have been removed as
 * much as possible for efficiency, in favor of drop-through switches.
 * (See "Note A" at the bottom of the file for equivalent code.)
 * If your compiler supports it, the "isLegalUTF8" call can be turned
 * into an inline function.
 */

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF16toUTF8 (
        const UTF16** sourceStart, const UTF16* sourceEnd,
        UTF8** targetStart, UTF8* targetEnd, ConversionFlags flags) {
    ConversionResult result = conversionOK;
    const UTF16* source = *sourceStart;
    UTF8* target = *targetStart;
    while (source < sourceEnd) {
        UTF32 ch;
        unsigned short bytesToWrite = 0;
        const UTF32 byteMask = 0xBF;
        const UTF32 byteMark = 0x80;
        const UTF16* oldSource = source; /* In case we have to back up because of target overflow. */
        ch = *source++;
        /* If we have a surrogate pair, convert to UTF32 first. */
        if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END) {
            /* If the 16 bits following the high surrogate are in the source buffer... */
            if (source < sourceEnd) {
                UTF32 ch2 = *source;
                /* If it's a low surrogate, convert to UTF32. */
                if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END) {
                    ch = ((ch - UNI_SUR_HIGH_START) << halfShift)
                        + (ch2 - UNI_SUR_LOW_START) + halfBase;
                    ++source;
                } else if (flags == strictConversion) { /* it's an unpaired high surrogate */
                    --source; /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                }
            } else { /* We don't have the 16 bits following the high surrogate. */
                --source; /* return to the high surrogate */
                result = sourceExhausted;
                break;
            }
        } else if (flags == strictConversion) {
            /* UTF-16 surrogate values are illegal in UTF-32 */
            if (ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END) {
                --source; /* return to the illegal value itself */
                result = sourceIllegal;
                break;
            }
        }
        /* Figure out how many bytes the result will require */
        if (ch < (UTF32)0x80) {      bytesToWrite = 1;
        } else if (ch < (UTF32)0x800) {     bytesToWrite = 2;
        } else if (ch < (UTF32)0x10000) {   bytesToWrite = 3;
        } else if (ch < (UTF32)0x110000) {  bytesToWrite = 4;
        } else {                            bytesToWrite = 3;
                                            ch = UNI_REPLACEMENT_CHAR;
        }

        target += bytesToWrite;
        if (target > targetEnd) {
            source = oldSource; /* Back up source pointer! */
            target -= bytesToWrite; result = targetExhausted; break;
        }
        switch (bytesToWrite) { /* note: everything falls through. */
            case 4: *--target = (UTF8)((ch | byteMark) & byteMask); ch >>= 6;
            case 3: *--target = (UTF8)((ch | byteMark) & byteMask); ch >>= 6;
            case 2: *--target = (UTF8)((ch | byteMark) & byteMask); ch >>= 6;
            case 1: *--target =  (UTF8)(ch | firstByteMark[bytesToWrite]);
        }
        target += bytesToWrite;
    }
    *sourceStart = source;
    *targetStart = target;
    return result;
}

/* --------------------------------------------------------------------- */

/*
 * Utility routine to tell whether a sequence of bytes is legal UTF-8.
 * This must be called with the length pre-determined by the first byte.
 * If not calling this from ConvertUTF8to*, then the length can be set by:
 *  length = trailingBytesForUTF8[*source]+1;
 * and the sequence is illegal right away if there aren't that many bytes
 * available.
 * If presented with a length > 4, this returns false.  The Unicode
 * definition of UTF-8 goes up to 4-byte sequences.
 */

static Boolean isLegalUTF8(const UTF8 *source, int length) {
    UTF8 a;
    const UTF8 *srcptr = source+length;
    switch (length) {
    default: return false;
        /* Everything else falls through when "true"... */
    case 4: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
    case 3: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
    case 2: if ((a = (*--srcptr)) > 0xBF) return false;

        switch (*source) {
            /* no fall-through in this inner switch */
            case 0xE0: if (a < 0xA0) return false; break;
            case 0xED: if (a > 0x9F) return false; break;
            case 0xF0: if (a < 0x90) return false; break;
            case 0xF4: if (a > 0x8F) return false; break;
            default:   if (a < 0x80) return false;
        }

    case 1: if (*source >= 0x80 && *source < 0xC2) return false;
    }
    if (*source > 0xF4) return false;
    return true;
}

/* --------------------------------------------------------------------- */

/*
 * Exported function to return whether a UTF-8 sequence is legal or not.
 * This is not used here; it's just exported.
 */
Boolean isLegalUTF8Sequence(const UTF8 *source, const UTF8 *sourceEnd) {
    int length = trailingBytesForUTF8[*source]+1;
    if (source+length > sourceEnd) {
        return false;
    }
    return isLegalUTF8(source, length);
}

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF8toUTF16 (
        const UTF8** sourceStart, const UTF8* sourceEnd,
        UTF16** targetStart, UTF16* targetEnd, ConversionFlags flags) {
    ConversionResult result = conversionOK;
    const UTF8* source = *sourceStart;
    UTF16* target = *targetStart;
    while (source < sourceEnd) {
        UTF32 ch = 0;
        unsigned short extraBytesToRead = trailingBytesForUTF8[*source];
        if (source + extraBytesToRead >= sourceEnd) {
            result = sourceExhausted; break;
        }
        /* Do this check whether lenient or strict */
        if (! isLegalUTF8(source, extraBytesToRead+1)) {
            result = sourceIllegal;
            break;
        }
        /*
         * The cases all fall through. See "Note A" below.
         */
        switch (extraBytesToRead) {
            case 5: ch += *source++; ch <<= 6; /* remember, illegal UTF-8 */
            case 4: ch += *source++; ch <<= 6; /* remember, illegal UTF-8 */
            case 3: ch += *source++; ch <<= 6;
            case 2: ch += *source++; ch <<= 6;
            case 1: ch += *source++; ch <<= 6;
            case 0: ch += *source++;
        }
        ch -= offsetsFromUTF8[extraBytesToRead];

        if (target >= targetEnd) {
            source -= (extraBytesToRead+1); /* Back up source pointer! */
            result = targetExhausted; break;
        }
        if (ch <= UNI_MAX_BMP) { /* Target is a character <= 0xFFFF */
            /* UTF-16 surrogate values are illegal in UTF-32 */
            if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
                if (flags == strictConversion) {
                    source -= (extraBytesToRead+1); /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                } else {
                    *target++ = UNI_REPLACEMENT_CHAR;
                }
            } else {
                *target++ = (UTF16)ch; /* normal case */
            }
        } else if (ch > UNI_MAX_UTF16) {
            if (flags == strictConversion) {
                result = sourceIllegal;
                source -= (extraBytesToRead+1); /* return to the start */
                break; /* Bail out; shouldn't continue */
            } else {
                *target++ = UNI_REPLACEMENT_CHAR;
            }
        } else {
            /* target is a character in range 0xFFFF - 0x10FFFF. */
            if (target + 1 >= targetEnd) {
                source -= (extraBytesToRead+1); /* Back up source pointer! */
                result = targetExhausted; break;
            }
            ch -= halfBase;
            *target++ = (UTF16)((ch >> halfShift) + UNI_SUR_HIGH_START);
            *target++ = (UTF16)((ch & halfMask) + UNI_SUR_LOW_START);
        }
    }
    *sourceStart = source;
    *targetStart = target;
    return result;
}

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF32toUTF8 (
        const UTF32** sourceStart, const UTF32* sourceEnd,
        UTF8** targetStart, UTF8* targetEnd, ConversionFlags flags) {
    ConversionResult result = conversionOK;
    const UTF32* source = *sourceStart;
    UTF8* target = *targetStart;
    while (source < sourceEnd) {
        UTF32 ch;
        unsigned short bytesToWrite = 0;
        const UTF32 byteMask = 0xBF;
        const UTF32 byteMark = 0x80;
        ch = *source++;
        if (flags == strictConversion ) {
            /* UTF-16 surrogate values are illegal in UTF-32 */
            if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
                --source; /* return to the illegal value itself */
                result = sourceIllegal;
                break;
            }
        }
        /*
         * Figure out how many bytes the result will require. Turn any
         * illegally large UTF32 things (> Plane 17) into replacement chars.
         */
        if (ch < (UTF32)0x80) {      bytesToWrite = 1;
        } else if (ch < (UTF32)0x800) {     bytesToWrite = 2;
        } else if (ch < (UTF32)0x10000) {   bytesToWrite = 3;
        } else if (ch <= UNI_MAX_LEGAL_UTF32) {  bytesToWrite = 4;
        } else {                            bytesToWrite = 3;
                                            ch = UNI_REPLACEMENT_CHAR;
                                            result = sourceIllegal;
        }

        target += bytesToWrite;
        if (target > targetEnd) {
            --source; /* Back up source pointer! */
            target -= bytesToWrite; result = targetExhausted; break;
        }
        switch (bytesToWrite) { /* note: everything falls through. */
            case 4: *--target = (UTF8)((ch | byteMark) & byteMask); ch >>= 6;
            case 3: *--target = (UTF8)((ch | byteMark) & byteMask); ch >>= 6;
            case 2: *--target = (UTF8)((ch | byteMark) & byteMask); ch >>= 6;
            case 1: *--target = (UTF8) (ch | firstByteMark[bytesToWrite]);
        }
        target += bytesToWrite;
    }
    *sourceStart = source;
    *targetStart = target;
    return result;
}

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF8toUTF32 (
        const UTF8** sourceStart, const UTF8* sourceEnd,
        UTF32** targetStart, UTF32* targetEnd, ConversionFlags flags) {
    ConversionResult result = conversionOK;
    const UTF8* source = *sourceStart;
    UTF32* target = *targetStart;
    while (source < sourceEnd) {
        UTF32 ch = 0;
        unsigned short extraBytesToRead = trailingBytesForUTF8[*source];
        if (source + extraBytesToRead >= sourceEnd) {
            result = sourceExhausted; break;
        }
        /* Do this check whether lenient or strict */
        if (! isLegalUTF8(source, extraBytesToRead+1)) {
            result = sourceIllegal;
            break;
        }
        /*
         * The cases all fall through. See "Note A" below.
         */
        switch (extraBytesToRead) {
            case 5: ch += *source++; ch <<= 6;
            case 4: ch += *source++; ch <<= 6;
            case 3: ch += *source++; ch <<= 6;
            case 2: ch += *source++; ch <<= 6;
            case 1: ch += *source++; ch <<= 6;
            case 0: ch += *source++;
        }
        ch -= offsetsFromUTF8[extraBytesToRead];

        if (target >= targetEnd) {
            source -= (extraBytesToRead+1); /* Back up the source pointer! */
            result = targetExhausted; break;
        }
        if (ch <= UNI_MAX_LEGAL_UTF32) {
            /*
             * UTF-16 surrogate values are illegal in UTF-32, and anything
             * over Plane 17 (> 0x10FFFF) is illegal.
             */
            if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
                if (flags == strictConversion) {
                    source -= (extraBytesToRead+1); /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                } else {
                    *target++ = UNI_REPLACEMENT_CHAR;
                }
            } else {
                *target++ = ch;
            }
        } else { /* i.e., ch > UNI_MAX_LEGAL_UTF32 */
            result = sourceIllegal;
            *target++ = UNI_REPLACEMENT_CHAR;
        }
    }
    *sourceStart = source;
    *targetStart = target;
    return result;
}

/* ---------------------------------------------------------------------

    Note A.
    The fall-through switches in UTF-8 reading code save a
    temp variable, some decrements & conditionals.  The switches
    are equivalent to the following loop:
        {
            int tmpBytesToRead = extraBytesToRead+1;
            do {
                ch += *source++;
                --tmpBytesToRead;
                if (tmpBytesToRead) ch <<= 6;
            } while (tmpBytesToRead > 0);
        }
    In UTF-8 writing code, the switches on "bytesToWrite" are
    similarly unrolled loops.

   --------------------------------------------------------------------- */

// original code is from here: http://www.codeproject.com/KB/string/UtfConverter.aspx
// License: The Code Project Open License (CPOL) http://www.codeproject.com/info/cpol10.aspx

// Changes by made to the original code (14/3/2011):
// 1. replaced #include "stdafx.h" with #include <string>
// 2. replaced "return resultstring;" with "return resultstring.c_str();" - it fixes a string length issue

namespace UtfConverter
{

        std::wstring FromUtf8(const std::string& utf8string)
        {
                size_t widesize = utf8string.length();
                if (sizeof(wchar_t) == 2)
                {
                        std::wstring resultstring;
                        resultstring.resize(widesize+1, L'\0');
                        const UTF8* sourcestart = reinterpret_cast<const UTF8*>(utf8string.c_str());
                        const UTF8* sourceend = sourcestart + widesize;
                        UTF16* targetstart = reinterpret_cast<UTF16*>(&resultstring[0]);
                        UTF16* targetend = targetstart + widesize;
                        ConversionResult res = ConvertUTF8toUTF16(&sourcestart, sourceend, &targetstart, targetend, strictConversion);
                        if (res != conversionOK)
                        {
                                throw std::runtime_error("Failed to convert UTF-8 string to UTF-16 string!");
                        }
                        *targetstart = 0;
                        return resultstring;
                }
                else if (sizeof(wchar_t) == 4)
                {
                        std::wstring resultstring;
                        resultstring.resize(widesize+1, L'\0');
                        const UTF8* sourcestart = reinterpret_cast<const UTF8*>(utf8string.c_str());
                        const UTF8* sourceend = sourcestart + widesize;
                        UTF32* targetstart = reinterpret_cast<UTF32*>(&resultstring[0]);
                        UTF32* targetend = targetstart + widesize;
                        ConversionResult res = ConvertUTF8toUTF32(&sourcestart, sourceend, &targetstart, targetend, strictConversion);
                        if (res != conversionOK)
                        {
                                throw std::runtime_error("Failed to convert UTF-8 string to UTF-16 string!");
                        }
                        *targetstart = 0;
                        return resultstring;
                }
                else
                {
                        throw std::runtime_error("Failed to convert UTF-8 string to UTF-16 string!");
                }
                return L"";
        }

        std::string ToUtf8(const std::wstring& widestring)
        {
                size_t widesize = widestring.length();

                if (sizeof(wchar_t) == 2)
                {
                        size_t utf8size = 3 * widesize + 1;
                        std::string resultstring;
                        resultstring.resize(utf8size, '\0');
                        const UTF16* sourcestart = reinterpret_cast<const UTF16*>(widestring.c_str());
                        const UTF16* sourceend = sourcestart + widesize;
                        UTF8* targetstart = reinterpret_cast<UTF8*>(&resultstring[0]);
                        UTF8* targetend = targetstart + utf8size;
                        ConversionResult res = ConvertUTF16toUTF8(&sourcestart, sourceend, &targetstart, targetend, strictConversion);
                        if (res != conversionOK)
                        {
                                throw std::runtime_error("Failed to convert UTF-16 string to UTF-8 string!");
                        }
                        *targetstart = 0;
                        return resultstring.c_str();
                }
                else if (sizeof(wchar_t) == 4)
                {
                        size_t utf8size = 4 * widesize + 1;
                        std::string resultstring;
                        resultstring.resize(utf8size, '\0');
                        const UTF32* sourcestart = reinterpret_cast<const UTF32*>(widestring.c_str());
                        const UTF32* sourceend = sourcestart + widesize;
                        UTF8* targetstart = reinterpret_cast<UTF8*>(&resultstring[0]);
                        UTF8* targetend = targetstart + utf8size;
                        ConversionResult res = ConvertUTF32toUTF8(&sourcestart, sourceend, &targetstart, targetend, strictConversion);
                        if (res != conversionOK)
                        {
                                throw std::runtime_error("Failed to convert UTF-16 string to UTF-8 string!");
                        }
                        *targetstart = 0;
                        return resultstring.c_str();
                }
                else
                {
                        throw std::runtime_error("Failed to convert UTF-16 string to UTF-8 string!");
                }
                return "";
        }
}


namespace Detail
{
    namespace{
        class ConversionTableCreate
        {
        private:
            VariantConversionTable table;
        public:
            ConversionTableCreate(const std::type_info& fromType, const std::type_info& toType, ConversionFn conversionFunc)
            {
                table.add(fromType, toType, conversionFunc);
            }
            ConversionTableCreate& operator()(const std::type_info& fromType, const std::type_info& toType, ConversionFn conversionFunc)
            {
                table.add(fromType, toType, conversionFunc);
                return *this;
            }

            operator VariantConversionTable()
            {
                return table;
            }
        };
    }

    void VariantConversionTable::add(const std::type_info& fromType, const std::type_info& toType, ConversionFn conversionFunction)
    {
        if (conversionFunction)
        {
            mConversions[&fromType][&toType] = conversionFunction;
        }
    }

    bool VariantConversionTable::canConvert(const std::type_info& fromType, const std::type_info& toType) const
    {
        ConversionMap::const_iterator from_itr = mConversions.find(&fromType);
        if (mConversions.end() != from_itr)
        {
            return from_itr->second.count(&toType) != 0;
        }
        return false;
    }

    template <typename FromType, typename ToType>
    bool VariantConvert(const void* inData, void* outData)
    {
        *((ToType*)(outData)) = (ToType)(*((const FromType*)inData));
        return true;
    }

    template <typename FromType>
    bool VariantConvertToRoString(const void* inData, void *outData)
    {
        std::ostringstream ss;
        ss <<*((const FromType*)inData);
        *(RoString*)outData = ss.str();
        return true;
    }

    template <>
    bool VariantConvertToRoString<std::string>(const void* inData, void* outData)
    {
        *(RoString*)outData = *(const std::string*)inData;
        return true;
    }

    template <>
    bool VariantConvertToRoString<std::wstring>(const void* inData, void* outData)
    {
        *(RoString*)outData = *(const std::wstring*)inData;
        return true;
    }

    template <typename ToType>
    bool VariantConvertFromRoString(const void* inData, void *outData)
    {
        ToType& outValue = *(ToType*)outData;
        const RoString& str = *(const RoString*)inData;
        std::istringstream ss(str.asUTF8());
        ss >> outValue;
        return !ss.fail();
    }

    template <>
    bool VariantConvertFromRoString<bool>(const void* inData, void* outData)
    {
        const RoString& str = *(const RoString*)inData;
        RoOptionalBool val = RoStringUtil::ParseBool(str);
        if (!val.is_initialized())
        {
            return false;
        }
        *(bool*)outData = val.get();
        return true;
    }

    template <>
    bool VariantConvertFromRoString<std::string>(const void* inData, void* outData)
    {
        *(std::string*)outData = *(const RoString*)inData;
        return true;
    }

    template <>
    bool VariantConvertFromRoString<std::wstring>(const void* inData, void* outData)
    {
        *(std::wstring*)outData = *(const RoString*)inData;
        return true;
    }

    template <typename FromType>
    bool VariantConvertToStr(const void *inData, void *outData)
    {
        std::ostringstream ss;
        ss<<*((const FromType*)inData);
        RoString string(ss.str());
        *(std::string*)outData = std::move(string.asUTF8());
        return true;
    }

    template <typename FromType>
    bool VariantConvertToWStr(const void *inData, void *outData)
    {
        std::wostringstream ss;
        ss<<*((const FromType*)inData);
        *(std::wstring*)outData = ss.str().c_str();
        return true;
    }

    template <typename ToType>
    bool VariantConvertFromStr(const void* inData, void* outData)
    {
        ToType& outValue = *(ToType*)outData;
        std::istringstream ss(*(const std::string*)inData);
        ss>>outValue;
        return !ss.fail();
    }

    template <>
    bool VariantConvertFromStr<bool>(const void *inData, void *outData)
    {
        static const std::string True = "true";
        static const std::string Yes = "yes";
        static const std::string False = "false";
        static const std::string No = "no";
        const std::string* inValue = (const std::string*)inData;
        if (inValue->empty())
        {
            return false;
        }
        std::string val = *inValue;
        std::transform(val.begin(), val.end(), val.begin(), tolower);
        bool& outValue = *(bool*)outData;
        if (True == val || Yes == val || val == "1")
        {
            outValue = true;
            return true;
        } else if (False == val || No == val || val == "0") {
            outValue = false;
            return true;
        }
        return false;
    }

    bool VariantConvertWStrToStr(const void* inData, void* outData)
    {
        *(std::string*)outData = UtfConverter::ToUtf8(*(const std::wstring*)inData);
        return true;
    }

    bool VariantConvertStrToWStr(const void* inData, void* outData)
    {
        *(std::wstring*)outData = UtfConverter::FromUtf8(*(const std::string*)inData);
        return true;
    }

#define CONVERT_FROM_ROSTRING_FUNC_REGISTER(ToType) (typeid(RoString), typeid(ToType), Detail::VariantConvertFromRoString<ToType>)
#define POD_CONVERT_FROM_STR_FUNC_REGISTER(ToType) (typeid(std::string), typeid(ToType), Detail::VariantConvertFromStr<ToType>)\
    CONVERT_FROM_ROSTRING_FUNC_REGISTER(ToType)
#define POD_CONVERT_FROM_STR_FUNC_REGISTER2(ToType) POD_CONVERT_FROM_STR_FUNC_REGISTER(ToType)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER(unsigned ToType)
#define POD_CONVERT_FROM_STR_FUNC_REGISTER_ALL POD_CONVERT_FROM_STR_FUNC_REGISTER(bool)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER2(char)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER2(short)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER2(int)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER2(long)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER2(long long)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER(float)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER(double)
}

Detail::VariantConversionTable RoVariant::sConversionTable = Detail::ConversionTableCreate
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(bool)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD2(char)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(int8)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(uint8)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD2(short)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(int16)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(uint16)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD2(int)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(int32)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(uint32)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD2(long)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD2(long long)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(int64)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(uint64)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(float)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(double)
    POD_CONVERT_FROM_STR_FUNC_REGISTER_ALL
    CONVERT_FROM_ROSTRING_FUNC_REGISTER(std::string)
    CONVERT_FROM_ROSTRING_FUNC_REGISTER(std::wstring)
    CONVERT_TO_ROSTRING_FUNC_REGISTER(std::string)
    CONVERT_TO_ROSTRING_FUNC_REGISTER(std::wstring)
    (typeid(std::string), typeid(std::wstring), Detail::VariantConvertStrToWStr)
    (typeid(std::wstring), typeid(std::string), Detail::VariantConvertWStrToStr);

