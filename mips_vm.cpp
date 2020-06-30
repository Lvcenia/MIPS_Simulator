#include "mips_vm.h"

MIPS_VM::MIPS_VM(QWidget* frontend,QObject *parent) : QObject(parent)
{
    this->frontend = frontend;
    this->Memory = new vmMemory(this);
        this->CPU = new vmCPU(this);
    this->CPU->setMemory(this->Memory);
    this->CPU->setFrontEnd(frontend);

}
