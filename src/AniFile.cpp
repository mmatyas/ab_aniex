#include "AniFile.h"

#include "BinaryFile.h"
#include "Bitmap.h"

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include <stdexcept>

void FileItem::read(BinaryFile& in)
{
    in.read_raw(signature_bytes, 4);
    length = in.read_u32();
    id = in.read_u16();

    signature = std::string((char*)signature_bytes, 4);
    start_pos = in.current_pos();
    end_pos = in.current_pos() + length;
}

AniFile::AniFile(const std::string& file_path)
    : file_path(file_path)
{
    BinaryFile in(file_path.c_str(), "r");
    if (!in.is_open())
        throw std::runtime_error("Could not open " + file_path);

    // read signature
    char signature_bytes[10] = {};
    in.read_raw(signature_bytes, 10);
    std::string signature = std::string(signature_bytes, 10);
    if (signature != "CHFILEANI ")
        throw std::runtime_error("This doesn't look like an ANI file to me (invalid ANI signature)");

    uint32_t file_length = in.read_u32();
    in.read_u16(); // file id

    // read items
    long file_end = in.current_pos() + file_length;
    while (in.current_pos() < file_end) {
        FileItem item;
        item.read(in);

        if (item.signature == "FRAM") {
            parse_frame(in, item);
        }
        else if (item.signature == "SEQ") {
            printf("warning: sequence detected, but this feature is not implemented yet...\n");
            // parse seq
        }
        else {
            in.skip_bytes(item.length);
        }
    }
}

AniFile::~AniFile() {}

void AniFile::parse_frame(BinaryFile& in, const FileItem& ani_item)
{
    bool has_name = false;
    bool has_data = false;

    std::string name("");
    Bitmap* tga_ptr = NULL;

    while (in.current_pos() < ani_item.end_pos) {
        FileItem item;
        item.read(in);


        if (item.signature == "FNAM") {
            if (!item.length)
                throw std::runtime_error("Frame name is empty!");

            char name_bytes[item.length];
            in.read_raw(name_bytes, item.length);
            name = std::string(name_bytes, item.length - 1);
            printf("info: found a frame: %s\n", name.c_str());

            has_name = true;
        }
        else if (item.signature == "CIMG") {
            //printf("  CIMG\n");
            parse_cimg(in, item, tga_ptr);
            has_data = true;
        }
        else {
            in.skip_bytes(item.length);
        }
    }

    if (!name.length())
        printf("warning: frame has no name, skipped\n");
    if (!has_data)
        printf("warning: frame has no data, skipped\n");

    if (has_name && has_data && tga_ptr) {
        tga_ptr->write_file(name);
        printf("info: %s exported successfully\n", name.c_str());
    }

    delete tga_ptr;
}

void AniFile::parse_cimg(BinaryFile& in, const FileItem& frame_item, Bitmap*& tga_ptr)
{
    if (frame_item.length < 32)
        throw std::runtime_error("CIMG is too small!");

    //
    // basic info (8B)
    //
    CImg img;
    img.type = in.read_u16();
    in.read_u16(); // unknown
    uint32_t additional_size = in.read_u32();
    //printf("  type: 0x%x, special header: %d bytes\n", img.type, additional_size);

    if (additional_size < 24)
        throw std::runtime_error("CIMG special header is too small!");
    if (additional_size > frame_item.end_pos - in.current_pos())
        throw std::runtime_error("CIMG size mismatch!");

    bool has_palette_header = false;
    size_t palette_size = 0;
    if (additional_size >= 32) {
        has_palette_header = true;

        if (additional_size > 32)
            palette_size = additional_size - 32;
        else
            throw std::runtime_error("CIMG palette missing!");
    }
    //printf("  palette size: %zu bytes\n", palette_size);

    //
    // image meta (16B)
    //
    in.read_u32(); // unknown
    img.width = in.read_u16();
    img.height = in.read_u16();
    img.hotspot_x = in.read_u16();
    img.hotspot_y = in.read_u16();
    img.keycolor_bytes = in.read_u16();
    in.read_u16(); // unknown

    switch (img.type) {
        case 0x04:
            img.bpp = 10;
            break;
        case 0x05:
            img.bpp = 24;
            if (palette_size > 0)
                throw std::runtime_error("CIMG type 5 cannot have palette!");
            break;
        case 0x0a:
            img.bpp = 4;
            if (palette_size != 64)
                throw std::runtime_error("CIMG type 10 must have 64 byte long header!");
            break;
        case 0x0b:
            if (palette_size != 1024)
                throw std::runtime_error("CIMG type 11 must have 1024 byte long header!");
            break;
        default:
            throw std::runtime_error("CIMG has unknown image type!");
    }

    printf("info: found an image: %d x %d @ %dbpp\n", img.width, img.height, img.bpp);
    if (img.type != 0x04)
        printf("warning: image is not in the default format, export may fail\n");

    //
    // palette data (vary)
    //
    if (has_palette_header) {
        in.read_u32(); // unknown
        in.read_u32(); // unknown

        if (img.bpp < 16)
            in.read_raw(tga_ptr->palette_data, tga_ptr->palette_size);

        if (palette_size > tga_ptr->palette_size)
            in.skip_bytes(palette_size - tga_ptr->palette_size);
    }

    //
    // special header (vary)
    //
    in.read_u16(); // unknown
    in.read_u16(); // unknown
    img.compressed_size = in.read_u32() - 12;
    img.uncompressed_size = in.read_u32();

    //
    // create TGA
    //
    delete tga_ptr;
    tga_ptr = new Bitmap(img);
    in.read_raw(tga_ptr->data, tga_ptr->data_size);

    //
    // item end
    //
    if (in.current_pos() + 1 > frame_item.end_pos) {
        // no terminator
    }
    else if (in.current_pos() + 1 < frame_item.end_pos) {
        in.read_u8();
        in.skip_bytes(frame_item.end_pos - in.current_pos() - 1);
    }
    else {
        if (in.read_u8() != 0xFF)
            in.read_u8();
    }
}
