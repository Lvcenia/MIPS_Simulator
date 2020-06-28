#include "vmmemory.h"

vmMemory::vmMemory(QObject *parent) : QObject(parent)
{

}

uint16_t vmMemory::GetValue(int address)
{
    if (address>MEMORY_UNIT_NUMBER || address <0) return 0;
    return this->MemorySpace[address];

}

uint32_t vmMemory::GetWordValue(int address)
{
    uint32_t res;
    if (address + 1>MEMORY_UNIT_NUMBER || address <0) return 0;
    auto high = MemorySpace[address];
    auto low = MemorySpace[address+1];

    res = high << 16 | low;
    return res;

}


void vmMemory::SetValue(int address, uint16_t value)
{
    if (address>MEMORY_UNIT_NUMBER || address <0) return;
    emit valueChanged(address, this->MemorySpace[address],value);
    this->MemorySpace[address] = value;

}

void vmMemory::SetWord(int address, uint32_t word)
{
    if (address + 1 > MEMORY_UNIT_NUMBER || address <0) return;
    emit valueChanged(address, this->MemorySpace[address],(uint16_t)((word & 0xffff0000) >> 16));
    this->MemorySpace[address] = (uint16_t)((word & 0xffff0000) >> 16);
    emit valueChanged(address+1, this->MemorySpace[address+1],(uint16_t)(word & 0x0000ffff));
    this->MemorySpace[address + 1] = (uint16_t)(word & 0x0000ffff);
}

void vmMemory::SetDword(int address, uint64_t dword)
{
    if(address + 4 > MEMORY_UNIT_NUMBER || address < 0) return;
    auto high = (uint32_t)(dword & 0xffffffff00000000) >> 32;
    SetWord(address,high);
    auto low = (uint32_t)(dword & 0x00000000ffffffff);
    SetWord(address+2,low);
}

uint16_t *vmMemory::GetAllMemoryData()
{
    return this->MemorySpace;

}
