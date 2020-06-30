#include "vmmemory.h"

vmMemory::vmMemory(QObject *parent) : QObject(parent)
{
    //this->MemorySpace = QVector<uint16_t>(MEMORY_UNIT_NUMBER);
}

uint16_t vmMemory::GetValue(unsigned int addressOfs,bool isOfs)
{
    if(isOfs)
        addressOfs += STATIC_DATA_SEG_START;
    if(this->VirtualMemoryReflection.contains(addressOfs))
        return this->VirtualMemoryReflection.value(addressOfs);
    else
        return 0;

    //if (addressOfs> MEMORY_UNIT_NUMBER || addressOfs <0) return 0;
    //return this->MemorySpace[addressOfs];

}

uint32_t vmMemory::GetWordValue(unsigned int addressOfs,bool isOfs)
{
    if(isOfs)
        addressOfs += STATIC_DATA_SEG_START;
    uint32_t res;
    if (addressOfs + 1> MEMORY_UNIT_NUMBER || addressOfs <STATIC_DATA_SEG_START) return 0;
    auto high = this->VirtualMemoryReflection.value(addressOfs);
    auto low = this->VirtualMemoryReflection.value(addressOfs+1);

    res = high << 16 | low;
    return res;

}

uint32_t vmMemory::GetInstrValue(unsigned int addressOfs,bool isOfs)
{
    if(isOfs)
        addressOfs += TEXT_SEGMENT_START;
    uint32_t res;
    if (addressOfs + 1> MEMORY_UNIT_NUMBER || addressOfs <TEXT_SEGMENT_START) return 0;
    auto high = this->VirtualMemoryReflection.value(addressOfs);
    auto low = this->VirtualMemoryReflection.value(addressOfs+1);

    res = high << 16 | low;
    return res;

}


void vmMemory::SetZjie(unsigned int addressOfs, uint16_t value)
{
    auto ofs = addressOfs;
    qDebug()<<"Setting " << value << "at " << addressOfs;
    addressOfs = addressOfs + STATIC_DATA_SEG_START;
    if (addressOfs > MEMORY_UNIT_NUMBER || addressOfs < STATIC_DATA_SEG_START) return;
    emit valueChanged(ofs, 0,value);
    //this->MemorySpace[addressOfs] = value;
    this->VirtualMemoryReflection.insert(addressOfs,value);

}

void vmMemory::SetWord(unsigned int addressOfs, uint32_t word)
{
        auto ofs = addressOfs;
    //qDebug()<<"Setting " << word << "at " << addressOfs;
    addressOfs = addressOfs + STATIC_DATA_SEG_START;
    if (addressOfs + 1 > MEMORY_UNIT_NUMBER || addressOfs <STATIC_DATA_SEG_START) return;
    emit valueChanged(ofs, 0,(uint16_t)((word & 0xffff0000) >> 16));
    //this->MemorySpace[addressOfs] = (uint16_t)((word & 0xffff0000) >> 16);
    this->VirtualMemoryReflection.insert(addressOfs,(uint16_t)((word & 0xffff0000) >> 16));
    emit valueChanged(ofs+1, 0,(uint16_t)(word & 0x0000ffff));
    //this->MemorySpace[addressOfs + 1] = (uint16_t)(word & 0x0000ffff);
    this->VirtualMemoryReflection.insert(addressOfs+1,(uint16_t)(word & 0x0000ffff));
}

void vmMemory::SetDword(unsigned int addressOfs, uint64_t dword)
{
   // addressOfs = addressOfs + STATIC_DATA_SEG_START;
   //if(addressOfs + 4 > MEMORY_UNIT_NUMBER || addressOfs < STATIC_DATA_SEG_START) return;
    auto high = (uint32_t)((dword & 0xffffffff00000000) >> 32);
    SetWord(addressOfs,high);
    auto low = (uint32_t)(dword & 0x00000000ffffffff);
    SetWord(addressOfs+2,low);
}

void vmMemory::SetInstr(unsigned int addressOfs, uint32_t instr)
{
        auto ofs = addressOfs;
    addressOfs = addressOfs + TEXT_SEGMENT_START;
    if(addressOfs < TEXT_SEGMENT_START || addressOfs >= STATIC_DATA_SEG_START) return;
    //emit valueChanged(addressOfs, this->VirtualMemoryReflection.value(addressOfs),(uint16_t)((instr & 0xffff0000) >> 16));
    //this->MemorySpace[addressOfs] = (uint16_t)((instr & 0xffff0000) >> 16);
    //emit valueChanged(addressOfs+1, this->VirtualMemoryReflection.value(addressOfs),(uint16_t)(instr & 0x0000ffff));
    //this->MemorySpace[addressOfs + 1] = (uint16_t)(instr & 0x0000ffff);
    this->VirtualMemoryReflection.insert(addressOfs,(uint16_t)((instr & 0xffff0000) >> 16));
    this->VirtualMemoryReflection.insert(addressOfs+1,(uint16_t)(instr & 0x0000ffff));
    emit instrChanged(ofs,0,instr);



}

uint32_t vmMemory::MemoryAlloc(int size)
{


}

