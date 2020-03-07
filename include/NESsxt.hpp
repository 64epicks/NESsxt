#ifndef NESSXT_HPP
#define NESSXT_HPP
#ifdef __cplusplus
#include <stddef.h>
#ifndef NO_NAMESPACE
#define NO_NAMESPACE
#endif
#include "6502.hpp"

enum Mappers
{
    NROM,
};
enum Mirroring
{
    HORIZONTAL,
    VERTICAL,
    BOTH,
};
struct EmulatorData
{
    enum Mappers mapper;

    unsigned char prg_rom_size;
    unsigned char chr_rom_size;

    enum Mirroring mirroring;
};
class Mapper
{
    public:
    struct EmulatorData data;
    char* rom;
    size_t length;

    virtual void configure(CPU *cpu, char* rom, size_t length);
    virtual unsigned char read(unsigned short);
    virtual void write(unsigned short, unsigned char);
};

// Mappers...
class M_NROM : public Mapper
{
    public:
    void configure(CPU *cpu, char* rom, size_t length);
    unsigned char read(unsigned short);
    void write(unsigned short, unsigned char);
};

class NESsxt
{
    public:
    NESsxt();
    NESsxt(const char*, size_t);
    ~NESsxt();

    void loadCartridge(const char*, size_t);
    const struct EmulatorData *getInfo();

    void powerUp();
    void runFrame();
    void cycle();

    unsigned char read(unsigned short);
    void write(unsigned short, unsigned char);

    private:
    void init();

    CPU *cpu;
    Mapper *mapper;

    int remainingCycles;
    char* ram;
};
#endif
#endif