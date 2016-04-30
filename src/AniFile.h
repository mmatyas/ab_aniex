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
    uint32_t start_pos;
    uint32_t end_pos;

    void read(BinaryFile&, uint32_t&);
};

class AniFile {
public:
    AniFile(const std::string& file_path);
    virtual ~AniFile();

private:
    std::list<AniFrame> frames;
    std::list<AniSequence> sequences;

    void parse_frame(BinaryFile&, uint32_t&, const FileItem&);
};

#endif // ANIFILE_H
