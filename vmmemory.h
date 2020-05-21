#ifndef VMMEMORY_H
#define VMMEMORY_H
#include<QObject>

class vmMemory: public QObject
{
    Q_OBJECT
public:
    explicit vmMemory(QObject *parent = nullptr);
    uint16_t GetValue(int address);
    void SetValue(int address,uint16_t value);
    void SetWord(int address,uint32_t word);
    void SetDword(int address,uint64_t dword);
    uint16_t* GetAllMemoryData();

private:
    uint16_t MemorySpace[0x7FFF];//内存大小63K

signals:
    void valueChanged(int address,uint16_t prevValue,uint16_t curValue);
};

#endif // VMMEMORY_H