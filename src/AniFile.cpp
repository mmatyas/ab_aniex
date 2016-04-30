#include "AniFile.h"

#include <stdio.h>
#include <stdint.h>

#include <stdexcept>

void FileItem::read(BinaryFile& in, uint32_t& read_pos)
{
    in.read_raw(signature_bytes, 4);
    length = in.read_u32();
    id = in.read_u16();

    read_pos += 10;

    signature = std::string((char*)signature_bytes, 4);
    start_pos = read_pos;
    end_pos = read_pos + length;
}

AniFile::AniFile(const std::string& file_path)
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

    // item counts
    unsigned PAL_count = 0;
    unsigned TPAL_count = 0;

    // read items
    uint32_t read_pos = 0;
    while (read_pos < file_length) {
        FileItem item;
        item.read(in, read_pos);

        if (item.signature == "FRAM") {
            printf("FRAM\n");
            parse_frame(in, read_pos, item);
        }
        else if (item.signature == "SEQ") {
            // parse seq
        }
        else {
            in.skip_bytes(item.length);
            read_pos += item.length;
        }
    }
}

AniFile::~AniFile() {}

void AniFile::parse_frame(BinaryFile& in, uint32_t& read_pos, const FileItem& ani_item)
{
    while (read_pos < ani_item.end_pos) {
        FileItem item;
        item.read(in, read_pos);

        if (item.signature == "FNAM") {
            if (!item.length)
                throw std::runtime_error("Frame name is empty!");

            char name_bytes[item.length];
            in.read_raw(name_bytes, item.length);
            read_pos += item.length;
            std::string name = std::string(name_bytes, item.length);
            printf("%s\n", name.c_str());
        }
        else if (item.signature == "CIMG") {
            // parse cimg
        }
        else {
            in.skip_bytes(item.length);
            read_pos += item.length;
        }
    }
}

