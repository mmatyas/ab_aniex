#ifndef BITMAP_H
#define BITMAP_H

#include <math.h>
#include <stdint.h>
#include <stddef.h>

#include "BinaryFile.h"
#include "CIMGInfo.h"

struct Bitmap {
    static const size_t BMP_HEADER_SIZE = 54;

    uint16_t bpp;
    uint16_t width;
    uint16_t height;

    uint8_t* palette_data;
    size_t palette_size;

    uint8_t* data;
    size_t data_size;

    Bitmap(const CImg&);
    virtual ~Bitmap();

    void write_file(const std::string filename);
};

#endif // BITMAP_H
