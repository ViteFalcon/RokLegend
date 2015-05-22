/* grf.h : main grf include
 * Blavla
 *
 */

#ifndef __GRF_H_INCLUDED
#define __GRF_H_INCLUDED

#define _LARGEFILE_SOURCE

#ifdef __C99
#error test
#endif

#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include "hash_tables.h"

#ifdef _WIN32
#   ifdef GRF_STATIC
#       define WIN32_GRFEXPORT extern
#   else
#       define WIN32_GRFEXPORT __declspec(dllexport)
#   endif
#endif

#ifndef GRF_NO_EXPORT
#   ifdef _WIN32
#       define GRFEXPORT WIN32_GRFEXPORT
#   else
#       define GRFEXPORT extern
#   endif
#else
#   define GRFEXPORT
#endif

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_REVISION 31

#ifdef _WIN32
#   define VERSION_TYPE "Win32"
#   define COMPILER_HINT_PACK
#else
#   define VERSION_TYPE "Unix"
#   define COMPILER_HINT_PACK __attribute__ ((__packed__))
#endif

#ifdef __USE_LARGEFILE
#define VERSION_EXTRA ", LargeFile"
#else
#define VERSION_EXTRA
#endif

#ifdef __DEBUG
#define VERSION_EXTRA2 ", Debug"
#else
#define VERSION_EXTRA2
#endif

#define VERSION_STRING "Gravity Ragnarok Files lib v%d.%d.%d (" VERSION_TYPE VERSION_EXTRA VERSION_EXTRA2 ") by MagicalTux <MagicalTux@ooKoo.org>\nBundled zlib version %s"

typedef struct grf_handler *grf_handle;
typedef struct grf_node *grf_node_handle;
typedef struct grf_treenode *grf_treenode_handle;
#define __LIBGRF_HAS_TYPEDEF

struct grf_node {
    grf_node_handle prev, next;
	grf_handle parent;
	grf_treenode_handle tree_parent;
	char *filename, flags;
	uint32_t size, len, len_aligned, pos, id;
	int cycle;
};

struct grf_treenode {
	bool is_dir;
	char *name;
	hash_table *subdir;
	grf_node_handle ptr;
	grf_treenode_handle parent;
};

struct grf_handler {
	uint32_t filecount, table_offset, table_size, wasted_space;
	uint32_t version;
	int fd;
	int compression_level;
	bool need_save, write_mode;
	grf_node_handle first_node;
	hash_table *fast_table;
	grf_treenode_handle root;
	bool (* callback)(void *, grf_handle, int, int, const char *);
	void *callback_etc;
	grf_node_handle*node_table;
};

#define GRF_HEADER_SIZE 0x2e /* sizeof(grf_header) */
#define GRF_HEADER_MAGIC "Master of Magic"
#define GRF_FILE_OUTPUT_VERISON 0x200
#define GRF_HASH_TABLE_SIZE 128
#define GRF_TREE_HASH_SIZE 32

/* values specific to all directories */
#define GRF_DIRECTORY_LEN 1094
#define GRF_DIRECTORY_LEN_ALIGNED 1812
#define GRF_DIRECTORY_SIZE 1372
#define GRF_DIRECTORY_OFFSET 1418

// should we use pragma pack ?
#pragma pack(push, 1)
struct grf_header {
    char header_magic[16] COMPILER_HINT_PACK; // "Master of Magic" + 0x00
    char header_key[14] COMPILER_HINT_PACK; // 0x01 -> 0x0e, or 0x00 -> 0x00 (no crypt)
    uint32_t offset COMPILER_HINT_PACK; // offset of file table
    uint32_t seed COMPILER_HINT_PACK;
    uint32_t filecount COMPILER_HINT_PACK; // Real filecount = filecount - seed - 7
    uint32_t version COMPILER_HINT_PACK; // 0x102 0x103 0x200 0xCACA
};

struct grf_table_entry_data {
	// file name (char + 0x00)
    uint32_t len COMPILER_HINT_PACK; // packed len
    uint32_t len_aligned COMPILER_HINT_PACK; // same, but with the alignment (?)
    uint32_t size COMPILER_HINT_PACK; // real file size (unpacked)
    uint8_t flags COMPILER_HINT_PACK; // file flags
    uint32_t pos COMPILER_HINT_PACK; // position in the grf
};
#pragma pack(pop)

int zlib_buffer_inflate(void *, int, void *, int); /* private: zlib.c */
int zlib_buffer_deflate(void *, int, void *, int, int); /* private: zlib.c */

#define MAX(a,b) ((a>b)?a:b)

#include "libgrf.h"

#endif /* __GRF_H_INCLUDED */

