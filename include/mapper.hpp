#ifndef MAPPER_HPP
#define MAPPER_HPP
#include <stddef.h>

enum Format
{
    FORMAT_ND,
    INES,
    NES2,
};
enum Mapper_ID
{
    MAPPER_NROM,
    MAPPER_MMC1,
    MAPPER_UXROM,
};
enum Cartridge_Mirroring
{
    MIRROR_HORIZONTAL,
    MIRROR_VERTICAL,
    MIRROR_BOTH, // Use cartridge provided VRAM
};
struct CartridgeData
{
    enum Format format;
    enum Mapper_ID mapper;
    enum Cartridge_Mirroring mirroring;
    bool hasRAM;
    
    size_t prg_rom_size;
    size_t chr_rom_size;
    
    char* rom;
    char* prg_rom_start;
    char* chr_rom_start;
};

#ifdef __cplusplus
class Mapper
{
    public:
    CartridgeData cartridge_info;

    virtual void configure();

    virtual unsigned char prg_read(unsigned short);
    virtual void          prg_write(unsigned short, unsigned char);

    virtual unsigned char chr_read(unsigned short);
    virtual void          chr_write(unsigned short, unsigned char);
};
Mapper *cartridgeParse(const char* data, size_t length);
void INES_parse(char* data, size_t length, CartridgeData *);
void NES2_parse(char* data, size_t length, CartridgeData *);

class NROM : public Mapper
{
    public:
    char* prg_ram;
    char* chr_ram; // Optional

    void configure();

    unsigned char prg_read(unsigned short);
    void          prg_write(unsigned short, unsigned char);

    unsigned char chr_read(unsigned short);
    void          chr_write(unsigned short, unsigned char);
};
class MMC1 : public Mapper
{
    public:
    char* prg_ram;

    void configure();

    unsigned char prg_read(unsigned short);
    void          prg_write(unsigned short, unsigned char);

    unsigned char chr_read(unsigned short);
    void          chr_write(unsigned short, unsigned char);
};
class UXROM : public Mapper
{
    public:
    unsigned char r;

    void configure();

    unsigned char prg_read(unsigned short);
    void          prg_write(unsigned short, unsigned char);

    unsigned char chr_read(unsigned short);
    void          chr_write(unsigned short, unsigned char);
};

#endif
#endif