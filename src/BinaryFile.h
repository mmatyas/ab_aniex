#ifndef FILEIO_H
#define FILEIO_H

#include <stdint.h>
#include <stdio.h>
#include <string>

class BinaryFile {
public:
    BinaryFile(const char* filename, const char* options);
    BinaryFile(const std::string& filename, const char* options);
    ~BinaryFile();

    bool is_open();
    void rewind();
    long current_pos() const;

    void skip_bytes(size_t);

    void write_u8(uint8_t);
    void write_u16(int16_t);
    void write_u32(int32_t);
    void write_raw(const void*, size_t);

    uint8_t read_u8();
    uint16_t read_u16();
    uint32_t read_u32();
    void read_raw(void*, size_t);

private:
    FILE* fp;

    void fread_or_exception(void*, size_t, size_t);
    void fwrite_or_exception(const void*, size_t, size_t);
};

#endif // FILEIO_H
