#ifndef VMCPU_H
#define VMCPU_H

#include <QObject>
#include <QHash>
#include <QDebug>
#include "vmmemory.h"

enum Instructions{
    LUi,
    ADD,
    ADDi,
    SUB,
    SLT,
    SLTi,
    SLTu,
    SLTiu,
    AND,
    ANDi,
    OR,
    ORi,
    XOR,
    XORi,
    NOR,
    SLL,
    SLLv,
    SRL,
    SRLv,
    SRA,
    SRAv,
    LW,
    LWx,
    LH,
    LHx,
    LHu,
    LHux,
    SW,
    SWx,
    SH,
    SHx,
    BEQ,
    BNE,
    BGEZAL,
    J,
    JAL,
    JR,
    JALr,
    MFC0,
    MTC0,
    ERET,
    SYSCALL
};

enum syscallFunctions{

};

class vmCPU : public QObject
{
    Q_OBJECT
public:
    explicit vmCPU(QObject *parent = nullptr);
    void ExecInstruction(const QString& Instr);
    void setMemory(vmMemory* m);

    bool isRunning = false;

private:
    int PC = 0;
    int RegistersValue[32] = {0};
    QHash<QString,Instructions> InstrStringToEnum;
    QHash<QString,int> RegDict;//$zero -> 0
    QStringList curInstrSet;
    void initInstrSet();
    void initRegDict();
    void initRegVals();
    vmMemory *mem = nullptr;
    void SysCallProcess(int code);




signals:
    void sysCallResultGot(const QString& result);
    void registerValueChanged(int number,QString name,int value);
    void currentInstrChanged(int PC);
    void ProcessStarted();
    void ProcessEnded();

public slots:
    void OnStartExcecution(QString instrs);
    void OnStepToNext();



};

#endif // VMCPU_H
