#include "Bitmap.h"

#include <stdexcept>
#include <string.h>

Bitmap::Bitmap(const CImg& img)
    : bpp(img.bpp)
    , width(img.width)
    , height(img.height)
    , palette_data(NULL)
    , palette_size(0)
    , data(NULL)
    , data_size(img.compressed_size)
{
    if (img.bpp < 16) {
        palette_size = 4 * (1 << img.bpp);
        palette_data = new uint8_t[palette_size];
    }

    data = new uint8_t[data_size];
}

Bitmap::~Bitmap()
{
    delete[] palette_data;
    delete[] data;
}

void Bitmap::write_file(const std::string filename)
{
    uint8_t tga_rle_header[12] = {
        0, 0, 10, 0, 0, 0, 0, 0, 0
    };

    BinaryFile out("./" + filename, "w");
    out.write_raw(tga_rle_header, 12);
    out.write_u16(width);
    out.write_u16(height);
    out.write_u8(16); // Targa 16, Image Pixel Size
    // Targa Image Descriptor Byte
    //   bit 5: screen origin bit
    out.write_u8(0x20);
    out.write_raw(data, data_size);
}
