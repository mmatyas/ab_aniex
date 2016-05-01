#include "BinaryFile.h"

#include <cstring>
#include <stdexcept>

BinaryFile::BinaryFile(const char* filename, const char* options)
    : fp(NULL)
{
    fp = fopen(filename, options);
}

BinaryFile::BinaryFile(const std::string& filename, const char* options)
    : fp(NULL)
{
    fp = fopen(filename.c_str(), options);
}

BinaryFile::~BinaryFile()
{
    if (fp)
        fclose(fp);
}

void BinaryFile::fread_or_exception(void* ptr, size_t size, size_t count)
{
    if (fread(ptr, size, count, fp) != count)
        throw std::runtime_error("File read error");
}

void BinaryFile::fwrite_or_exception(const void* ptr, size_t size, size_t count)
{
    if (fwrite(ptr, size, count, fp) != count)
        throw std::runtime_error("File write error");
}

bool BinaryFile::is_open()
{
    return fp;
}

void BinaryFile::rewind()
{
    if (fp)
        ::rewind(fp);
}

long BinaryFile::current_pos() const
{
    if (fp)
        return ftell(fp);

    return -1;
}

void BinaryFile::skip_bytes(size_t amount)
{
    if (0 != fseek(fp, amount, SEEK_CUR))
        throw std::runtime_error("File seek error");
}

void BinaryFile::write_u8(uint8_t value)
{
    fwrite_or_exception(&value, sizeof(uint8_t), 1);
}

void BinaryFile::write_u16(int16_t value)
{
    fwrite_or_exception(&value, sizeof(uint16_t), 1);
}

void BinaryFile::write_u32(int32_t value)
{
    fwrite_or_exception(&value, sizeof(uint32_t), 1);
}

void BinaryFile::write_raw(const void* source, size_t size)
{
    fwrite_or_exception(source, size, 1);
}

uint8_t BinaryFile::read_u8()
{
    uint8_t b;
    fread_or_exception(&b, sizeof(uint8_t), 1);
    return b;
}

uint16_t BinaryFile::read_u16()
{
    uint16_t in;
    fread_or_exception(&in, sizeof(uint16_t), 1);
    return in;
}

uint32_t BinaryFile::read_u32()
{
    uint32_t in;
    fread_or_exception(&in, sizeof(uint32_t), 1);
    return in;
}

void BinaryFile::read_raw(void* target, size_t size)
{
    fread_or_exception(target, size, 1);
}
