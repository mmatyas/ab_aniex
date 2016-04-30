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

void BinaryFile::write_i8(int8_t value)
{
    fwrite_or_exception(&value, sizeof(int8_t), 1);
}

void BinaryFile::write_u8(uint8_t value)
{
    fwrite_or_exception(&value, sizeof(uint8_t), 1);
}

void BinaryFile::write_i16(int16_t value)
{
#ifdef BYTEORDER_BIG_ENDIAN
    int32_t tmp = value;

    ((char*)&out)[0] = ((char*)&tmp)[1];
    ((char*)&out)[1] = ((char*)&tmp)[0];
#endif

    fwrite_or_exception(&value, sizeof(int16_t), 1);
}

void BinaryFile::write_i32(int32_t value)
{
#ifdef BYTEORDER_BIG_ENDIAN
    int32_t tmp = value;

    ((char*)&out)[0] = ((char*)&tmp)[3];
    ((char*)&out)[1] = ((char*)&tmp)[2];
    ((char*)&out)[2] = ((char*)&tmp)[1];
    ((char*)&out)[3] = ((char*)&tmp)[0];
#endif

    fwrite_or_exception(&value, sizeof(int32_t), 1);
}

void BinaryFile::write_bool(bool value)
{
    write_u8(value);
}

void BinaryFile::write_float(float value)
{
#ifdef BYTEORDER_BIG_ENDIAN
    float tmp = out;

    ((char*)&out)[0] = ((char*)&tmp)[3];
    ((char*)&out)[1] = ((char*)&tmp)[2];
    ((char*)&out)[2] = ((char*)&tmp)[1];
    ((char*)&out)[3] = ((char*)&tmp)[0];
#endif

    fwrite_or_exception(&value, sizeof(float), 1);
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

int8_t BinaryFile::read_i8()
{
    int8_t b;
    fread_or_exception(&b, sizeof(int8_t), 1);
    return b;
}

bool BinaryFile::read_bool()
{
    return read_u8();
}

int16_t BinaryFile::read_i16()
{
    int16_t in;
    fread_or_exception(&in, sizeof(int16_t), 1);

#ifdef BYTEORDER_BIG_ENDIAN
    int32_t tmp = in;

    ((char*)&in)[0] = ((char*)&tmp)[1];
    ((char*)&in)[1] = ((char*)&tmp)[0];
#endif

    return in;
}

uint16_t BinaryFile::read_u16()
{
    uint16_t in;
    fread_or_exception(&in, sizeof(uint16_t), 1);

#ifdef BYTEORDER_BIG_ENDIAN
    uint32_t tmp = in;

    ((char*)&in)[0] = ((char*)&tmp)[1];
    ((char*)&in)[1] = ((char*)&tmp)[0];
#endif

    return in;
}

int32_t BinaryFile::read_i32()
{
    int32_t in;
    fread_or_exception(&in, sizeof(int32_t), 1);

#ifdef BYTEORDER_BIG_ENDIAN
    int32_t tmp = in;

    ((char*)&in)[0] = ((char*)&tmp)[3];
    ((char*)&in)[1] = ((char*)&tmp)[2];
    ((char*)&in)[2] = ((char*)&tmp)[1];
    ((char*)&in)[3] = ((char*)&tmp)[0];
#endif

    return in;
}

uint32_t BinaryFile::read_u32()
{
    uint32_t in;
    fread_or_exception(&in, sizeof(uint32_t), 1);

#ifdef BYTEORDER_BIG_ENDIAN
    uint32_t tmp = in;

    ((char*)&in)[0] = ((char*)&tmp)[3];
    ((char*)&in)[1] = ((char*)&tmp)[2];
    ((char*)&in)[2] = ((char*)&tmp)[1];
    ((char*)&in)[3] = ((char*)&tmp)[0];
#endif

    return in;
}

float BinaryFile::read_float()
{
    float in;
    fread_or_exception(&in, sizeof(float), 1);

#ifdef BYTEORDER_BIG_ENDIAN
    float tmp = in;

    ((char*)&in)[0] = ((char*)&tmp)[3];
    ((char*)&in)[1] = ((char*)&tmp)[2];
    ((char*)&in)[2] = ((char*)&tmp)[1];
    ((char*)&in)[3] = ((char*)&tmp)[0];
#endif

    return in;
}

void BinaryFile::read_raw(void* target, size_t size)
{
    fread_or_exception(target, size, 1);
}
