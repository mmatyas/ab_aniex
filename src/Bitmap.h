#ifndef BITMAP_H
#define BITMAP_H

#include <math.h>
#include <stdint.h>
#include <stddef.h>

#include "BinaryFile.h"

struct Bitmap {
    static const size_t BMP_HEADER_SIZE = 54;

    uint16_t bpp;
    uint16_t width;
    uint16_t height;
    size_t bytes_per_line;
    size_t padding_per_line;
    size_t palette_size;
    size_t data_size;
    size_t palette_location;
    size_t data_location;
    uint8_t* data;

    Bitmap(uint16_t img_w, uint16_t img_h, uint16_t img_bpp);
    virtual ~Bitmap();

    void decode_cimg_data(BinaryFile& in, const size_t max_length);
};

#endif // BITMAP_H
