#ifndef VMMEMORY_H
#define VMMEMORY_H
#define MEMORY_UNIT_NUMBER 0x7fffffff
#define TEXT_SEGMENT_START 0x00400000
#define STATIC_DATA_SEG_START 0x10010000
#define $gp_ADDRESS 0x10008000
#define DYNAMIC_DATA_SEG_START 0x1000ffff

#include<QObject>
#include<QVector>
#include<QHash>
#include<QDebug>

class vmMemory: public QObject
{
    Q_OBJECT
public:
    ~vmMemory(){
    }
    explicit vmMemory(QObject *parent = nullptr);
    uint16_t GetValue(unsigned int addressOfs,bool isOfs = false);
    //数据段
    uint32_t GetWordValue(unsigned int addressOfs,bool isOfs = false);
    //代码段
    uint32_t GetInstrValue(unsigned int addressOfs,bool isOfs = false);
    void SetZjie(unsigned int addressOfs,uint16_t value);
    void SetWord(unsigned int addressOfs,uint32_t word);
    void SetDword(unsigned int addressOfs,uint64_t dword);
    void SetInstr(unsigned int addressOfs,uint32_t instr);
    uint32_t MemoryAlloc(int size);


    //
    QHash<int,uint16_t> VirtualMemoryReflection;

private:
    QVector<uint16_t> MemorySpace;//物理内存

signals:
    void valueChanged(int address,uint16_t prevValue,uint16_t curValue);
    void instrChanged(int address,uint32_t prevValue,uint32_t curValue);
};

#endif // VMMEMORY_H
