#include <6502.hpp>
#include <mapper.hpp>
#include <iostream>
#include <streambuf>
#include <fstream>
#include <string>

CPU* processor;
Mapper *mp;
char ram[0x800];

const char* file = "spacerace.nes";
unsigned char read(unsigned short address)
{
    switch (address) {
    case 0x0000 ... 0x1FFF:
        return ram[address % 0x800];
    case 0x6000 ... 0xFFFF:
        return mp->prg_read(address);
    };
    return 0;
}
void write(unsigned short address, unsigned char value)
{
    switch (address) {
    case 0x0000 ... 0x1FFF:
        ram[address % 0x800] = value;
        break;
    case 0x6000 ... 0xFFFF:
        return mp->prg_write(address, value);
    }
}

int main()
{
    std::ifstream t(file);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    processor = new CPU(read, write);
    mp = cartridgeParse(str.c_str(), str.length());

    processor->run();
}