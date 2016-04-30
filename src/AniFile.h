#ifndef ANIFILE_H
#define ANIFILE_H

#include "AniFrame.h"
#include "AniSequence.h"
#include "BinaryFile.h"

#include <list>
#include <string>

struct FileItem {
    // from file
    uint8_t signature_bytes[4];
    uint32_t length;
    uint16_t id;

    // calculated
    std::string signature;
    long start_pos;
    long end_pos;

    void read(BinaryFile&);
};

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

class AniFile {
public:
    AniFile(const std::string& file_path);
    virtual ~AniFile();

private:
    std::list<AniFrame> frames;
    std::list<AniSequence> sequences;

    void parse_frame(BinaryFile&, const FileItem&);
    void parse_cimg(BinaryFile&, const FileItem&);
};

#endif // ANIFILE_H
