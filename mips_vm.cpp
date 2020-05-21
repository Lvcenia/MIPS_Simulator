#include "mips_vm.h"

MIPS_VM::MIPS_VM(QObject *parent) : QObject(parent)
{

    this->Memory = new vmMemory(this);
        this->CPU = new vmCPU(this);
    this->CPU->setMemory(this->Memory);

}
