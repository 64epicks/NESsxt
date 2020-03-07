#include <mapper.hpp>
#include <string.h>

void INES_parse(char* data, size_t length, CartridgeData *cd)
{
    if (length < 16)
        return;
    if (data[0] != 'N' || data[1] != 'E' || data[2] != 'S' || data[3] != 0x1A)
        return;
    if ((data[7] & 0x0C) == 0x08)
        return;
    
    cd->rom = data;
    cd->prg_rom_size = 0x4000 * data[4];
    cd->chr_rom_size = 0x2000 * data[5];
    size_t totalSize = 16 + cd->prg_rom_size + cd->chr_rom_size;
    bool trainer = false;
    if (data[6] & 4) {
        trainer = true;
        totalSize += 512;
    }
    if (length < totalSize)
        return;

    if (!trainer)
        cd->prg_rom_start = &data[16];
    else
        cd->prg_rom_start = &data[528];
    cd->chr_rom_start = &cd->prg_rom_start[cd->prg_rom_size];

    if (data[6] & 8) {
        cd->mirroring = MIRROR_BOTH;
    }
    else {
        if (data[6] & 1) {
            cd->mirroring = MIRROR_VERTICAL;
        }
        else {
            cd->mirroring = MIRROR_HORIZONTAL;
        }
    }

    unsigned char mpi = (data[6] >> 4) & 0x0F;
    mpi |= data[7] & 0xF0;
    switch(mpi) {
    case 0x00:
        cd->mapper = MAPPER_NROM;
        break;
    case 0x01:
        cd->mapper = MAPPER_MMC1;
        break;
    case 0x02:
        cd->mapper = MAPPER_UXROM;
        break;
    default:
        return;
    }

    cd->format = INES;
    cd->hasRAM = false;
}
void NES2_parse(char* data, size_t length, CartridgeData *cd)
{
    if (length < 16)
        return;
    if (data[0] != 'N' || data[1] != 'E' || data[2] != 'S' || data[3] != 0x1A)
        return;
    if ((data[7] & 0x0C) != 0x08)
        return;
}

Mapper *cartridgeParse(const char* data, size_t length)
{
    if (length < 16)
        return nullptr;
    if (data[0] != 'N' || data[1] != 'E' || data[2] != 'S' || data[3] != 0x1A)
        return nullptr;

    char* dc = new char[length];
    memcpy(dc, data, length);
    CartridgeData cd;
    cd.format = FORMAT_ND;
    if ((data[7] & 0x0C) == 0x08) {
        NES2_parse(dc, length, &cd);
    }
    else {
        INES_parse(dc, length, &cd);
    }
    if (cd.format == FORMAT_ND)
        return nullptr;

    Mapper *mp;
    switch (cd.mapper) {
    case MAPPER_NROM:
        mp = new NROM;
        break;
    case MAPPER_MMC1:
        mp = new MMC1;
        break;
    case MAPPER_UXROM:
        mp = new UXROM;
        break;
    default:
        return nullptr;
    }
    mp->cartridge_info = cd;

    mp->configure();
    return mp;
}

void Mapper::configure()
{

}
unsigned char Mapper::prg_read(unsigned short address)
{
    return 0;
}
void Mapper::prg_write(unsigned short address, unsigned char value)
{

}
unsigned char Mapper::chr_read(unsigned short address)
{
    return 0;
}
void Mapper::chr_write(unsigned short address, unsigned char value)
{

}