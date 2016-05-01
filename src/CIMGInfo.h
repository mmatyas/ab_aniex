#ifndef CIMGINFO_H
#define CIMGINFO_H

#include <stddef.h>
#include <stdint.h>

struct CImg {
    uint16_t type;
    uint16_t width;
    uint16_t height;
    uint16_t hotspot_x;
    uint16_t hotspot_y;
    uint16_t keycolor_bytes;
    uint16_t bpp;
    size_t compressed_size;
    size_t uncompressed_size;
};

#endif // CIMGINFO_H
