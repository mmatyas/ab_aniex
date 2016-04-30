#include "Bitmap.h"

#include <stdexcept>
#include <string.h>

Bitmap::Bitmap(uint16_t img_w, uint16_t img_h, uint16_t img_bpp)
    : bpp(img_bpp)
    , width(img_w)
    , height(img_h)
    , data(NULL)
{
    bytes_per_line = ceil(img_w * img_bpp / 8.0);
    padding_per_line = (4 - bytes_per_line % 4) % 4;

    palette_size = 0;
    if (img_bpp < 16)
        palette_size = 4 * (1 << img_bpp);

    palette_location = BMP_HEADER_SIZE;
    data_location = palette_location + palette_size;

    data_size = BMP_HEADER_SIZE + palette_size + bytes_per_line * height;
    data = new uint8_t[data_size];

    unsigned i = 0;
    data[i++] = 'B';
    data[i++] = 'M';
    data[i++] = (data_size & 0xFF);
    data[i++] = ((data_size >> 8) & 0xFF);
    data[i++] = ((data_size >> 16) & 0xFF);
    data[i++] = ((data_size >> 24) & 0xFF);
    i += 4; // reserved 4 bytes
    data[i++] = (data_location & 0xFF);
    data[i++] = ((data_location >> 8) & 0xFF);
    data[i++] = ((data_location >> 16) & 0xFF);
    data[i++] = ((data_location >> 24) & 0xFF);
    data[i++] = 40;
    i += 3; // 3 empty bytes
    data[i++] = (width & 0xFF);
    data[i++] = ((width >> 8) & 0xFF);
    data[i++] = ((width >> 16) & 0xFF);
    data[i++] = ((width >> 24) & 0xFF);
    data[i++] = (height & 0xFF);
    data[i++] = ((height >> 8) & 0xFF);
    data[i++] = ((height >> 16) & 0xFF);
    data[i++] = ((height >> 24) & 0xFF);
    data[i++] = 1;
    i++;
    data[i++] = (bpp & 0xFF);
    data[i++] = ((bpp >> 8) & 0xFF);
}

Bitmap::~Bitmap()
{
    delete[] data;
}

void Bitmap::decode_cimg_data(BinaryFile& in, const size_t max_length)
{
    size_t bytes_read = 0;

    // Run-Length Encoding
    bool packet_is_rle = false;
    size_t bytes_per_rle_unit = ceil(bpp / 8.0);
    uint8_t rle_unit_bytes[bytes_per_rle_unit];
    uint8_t packet_counter = 0;


    unsigned current_position;
    for (unsigned y = 0; y < height; y++) {
        current_position =
            (BMP_HEADER_SIZE + palette_size) +
            (height - y - 1) * bytes_per_line;

        // note: see the TGA file specifications
        for (unsigned x = 0; x < width; x++) {
            // if the count is 0, the new block starts (raw or RLE)
            if (packet_counter == 0) {
                if (bytes_read + 1 > max_length)
                    throw std::runtime_error("CIMG data format error #1!");

                packet_counter = in.read_u8();
                bytes_read++;

                // start of a RLE block
                if (packet_counter & 0x80) {
                    packet_counter &= ~0x80; // unset the bit
                    packet_is_rle = true;

                    if (bytes_read + bytes_per_rle_unit > max_length)
                        throw std::runtime_error("CIMG data format error #2!");

                    in.read_raw(rle_unit_bytes, bytes_per_rle_unit);
                    bytes_read += bytes_per_rle_unit;
                }
                // start of a raw block
                else {
                    packet_is_rle = false;
                    if (bytes_read + packet_counter * bytes_per_rle_unit > max_length)
                        throw std::runtime_error("CIMG data format error #3!");
                }
            }
            // otherwise, the current block is still running
            else {
                packet_counter--;
            }


            if (current_position + bytes_per_rle_unit > data_size)
                throw std::runtime_error("CIMG data format error #4!");

            if (packet_is_rle) {
                memcpy(data + current_position, rle_unit_bytes, bytes_per_rle_unit);
            }
            else {
                in.read_raw(data + current_position, bytes_per_rle_unit);
                current_position += bytes_per_rle_unit;
                bytes_read += bytes_per_rle_unit;
            }
        }
    }

    printf("  %zu bytes decoded successfully\n", bytes_read);
}
