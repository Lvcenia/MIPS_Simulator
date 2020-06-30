#ifndef VMCPU_H
#define VMCPU_H

#include <QObject>
#include <QHash>
#include <QDebug>
#include "vmmemory.h"
#include "n_assembler.h"
#include <QInputDialog>
#include "CharData.h"
#include <QTextCodec>

//读入的指令类型(标准MIPS编译器编译的结果为StandardMIPS， 自制汇编器的编译结果为ZPC)
enum InstrMode{
    ZPC,
    StandardMIPS
};

enum Instructions{
    LUi,
    ADD,
    ADDi,
    ADDiu,
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
    MFCO,
    MTCO,
    ERET,
    SYSCALL,
    MUL,
    MULT,
    DIV,
    DIVu,
    MFHi,
    MLFO,
    MTHi,
    MTLO,
    SUBu,
    BGEZ,
    BGTZ,
    BLEZ,
    BLTZ,
    BLTZAL
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
    void setFrontEnd(QWidget* front);
    bool isRunning = false;
    void SetInstrMode(InstrMode mode);

private:
    QWidget *frontend;
    int PC = 0;
    int RegistersValue[32] = {0};
    QHash<QString,Instructions> InstrStringToEnum;
    QHash<QString,int> RegDict;//$zero -> 0
    QHash<int,QString> curInstrSet;
    void initInstrSet();
    void initRegDict();
    void initRegVals();
    vmMemory *mem = nullptr;
    void SysCallProcess(int code);
    QString GetSystemInput();
    int ProcessEndAddress = 0;
    //在内存里存储一个字符串或者字符 返回存储的首地址
    uint32_t StoreInputString(const QString& str);
    QString toHex(int num);
    InstrMode currentMode = StandardMIPS;
    int lo;
    int hi;



    template<typename T>
    void SystemPrint(T value)
    {
        QString res = QString("%1").arg(value);
        emit sysCallResultGot(res);
    }




signals:
    void sysCallResultGot(const QString& result);
    void registerValueChanged(int number,QString name,int value);
    void currentInstrChanged(int PC);
    void ProcessStarted();
    void ProcessEnded();


public slots:
    void OnStartExcecution(QString instrs);
    void OnStepToNext();
    void OnProcessEnd();



};

#endif // VMCPU_H
