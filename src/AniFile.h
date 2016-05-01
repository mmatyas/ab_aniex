#ifndef ANIFILE_H
#define ANIFILE_H

#include <list>
#include <stdint.h>
#include <string>

struct Bitmap;
struct BinaryFile;

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

class AniFile {
public:
    AniFile(const std::string& file_path);
    virtual ~AniFile();

private:
    std::string file_path;

    void parse_frame(BinaryFile&, const FileItem&);
    void parse_cimg(BinaryFile&, const FileItem&, Bitmap*&);
};

#endif // ANIFILE_H
