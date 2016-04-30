#ifndef ANIFILE_H
#define ANIFILE_H

#include "AniFrame.h"
#include "AniSequence.h"

#include <list>
#include <string>

class AniFile {
public:
    AniFile(const std::string& file_path);
    virtual ~AniFile();

private:
    std::list<AniFrame> frames;
    std::list<AniSequence> sequences;
};

#endif // ANIFILE_H
