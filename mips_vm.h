#ifndef MIPS_VM_H
#define MIPS_VM_H

#include <QObject>
#include "vmcpu.h"
#include "vmmemory.h"

class MIPS_VM : public QObject
{
    Q_OBJECT
public:
    explicit MIPS_VM(QWidget* frontend,QObject *parent = nullptr);

    vmCPU* CPU;
    vmMemory* Memory;
    QWidget *frontend;

signals:

public slots:


};

#endif // MIPS_VM_H
