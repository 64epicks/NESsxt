#include <mapper.hpp>

void NROM::configure()
{
    prg_ram = new char[4096];
    if (cartridge_info.chr_rom_size == 0)
        chr_ram = new char[8192];
    else
        chr_ram = nullptr;

    cartridge_info.hasRAM = true;
}

unsigned char NROM::prg_read(unsigned short address)
{
    switch (address) {
    case 0x6000 ... 0x7FFF:
        return prg_ram[address - 0x6000];
    case 0x8000 ... 0xBFFF:
        return cartridge_info.prg_rom_start[address - 0x8000];
    case 0xC000 ... 0xFFFF:
        if (cartridge_info.prg_rom_size == 32768)
            return cartridge_info.prg_rom_start[address - 0x8000];
        else
            return cartridge_info.prg_rom_start[address - 0x4000 - 0x8000];
    }
    return 0;
}
void NROM::prg_write(unsigned short address, unsigned char value)
{
    switch (address) {
    case 0x6000 ... 0x7FFF:
        prg_ram[address - 0x6000] = value;
    }
}

unsigned char NROM::chr_read(unsigned short address)
{
    switch (address) {
    case 0x0000 ... 0x1FFF:
        if (cartridge_info.chr_rom_size == 0)
            return chr_ram[address];
        else
            return cartridge_info.chr_rom_start[address];
    }
    return 0;
}
void NROM::chr_write(unsigned short address, unsigned char value)
{
    switch (address) {
    case 0x0000 ... 0x1FFF:
        if (cartridge_info.chr_rom_size == 0)
            chr_ram[address] = value;
        break;
    }
}