#include <NESsxt.hpp>
#include <string.h>

const int TOTAL_CYCLES = 29781;

NESsxt *c;
unsigned char rd(unsigned short address)
{
    return c->read(address);
}
void wr(unsigned short address, unsigned char value)
{
    c->write(address, value);
}
unsigned char NESsxt::read(unsigned short address)
{
    switch (address) {
    case 0x0000 ... 0x1FFF: return ram[address % 0x800];
    default: return mapper->read(address); 
    }
}
void NESsxt::write(unsigned short address, unsigned char value)
{
    switch (address) {
    case 0x0000 ... 0x1FFF: 
        ram[address % 0x800] = value;
        return;
    default: return mapper->write(address, value);
    }
}

void NESsxt::init()
{
    c = this;
    cpu = new CPU(rd, wr);
    mapper = new Mapper; // Placeholder
    ram = new char[0x800];
    remainingCycles = 0;
    memset(ram, 0, 0x800);
}
NESsxt::NESsxt()
{
    init();
}
NESsxt::NESsxt(const char* data, size_t length)
{
    init();
    loadCartridge(data, length);
}
NESsxt::~NESsxt()
{
    delete cpu;
    delete mapper;
    delete ram;
}
void NESsxt::loadCartridge(const char* data, size_t length)
{
    if (length < 16)
        return;
    if (strcmp(data, "NES") != 0 || data[3] != 0x1A)
        return;

    unsigned char mid = data[6] >> 4;
    mid |= data[7] & 0xF0;
    switch (mid) {
    case 0x00: 
        delete mapper;
        mapper = new M_NROM;
    default:
        return;
    }
    mapper->configure(cpu, strdup(data), length);
}

void NESsxt::cycle()
{
    cpu->cycle();
}

Mapper *getMapper(const char* rom, size_t length)
{
    unsigned char mid = rom[6] >> 4;
    mid |= rom[7] & 0xF0;
    switch (mid) {
    case 0x00: return new M_NROM;
    }
    return nullptr;
}