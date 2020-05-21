#include "vmmemory.h"

vmMemory::vmMemory(QObject *parent) : QObject(parent)
{

}

uint16_t vmMemory::GetValue(int address)
{
    if (address>0x7FFF || address <0) return 0;
    return this->MemorySpace[address];

}

void vmMemory::SetValue(int address, uint16_t value)
{
    if (address>0x7FFF || address <0) return;
    emit valueChanged(address, this->MemorySpace[address],value);
    this->MemorySpace[address] = value;

}

void vmMemory::SetWord(int address, uint32_t word)
{
    if (address + 1 >0x7FFF || address <0) return;
    emit valueChanged(address, this->MemorySpace[address],(uint16_t)((word & 0xffff0000) >> 16));
    this->MemorySpace[address] = (uint16_t)((word & 0xffff0000) >> 16);
    emit valueChanged(address+1, this->MemorySpace[address+1],(uint16_t)(word & 0x0000ffff));
    this->MemorySpace[address + 1] = (uint16_t)(word & 0x0000ffff);
}

void vmMemory::SetDword(int address, uint64_t dword)
{

}

uint16_t *vmMemory::GetAllMemoryData()
{
    return this->MemorySpace;

}
