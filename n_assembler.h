#ifndef N_ASSEMBLER_H
#define N_ASSEMBLER_H

#include <QObject>
#include <QHash>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QRegExp>
#include <QMessageBox>
#include <QStack>
#include "Instruction_Base.h"

enum InstrCheckType
{
    Correct,
    WrongInstrName,
    WrongParamNum,
    UndefinedLabel,
    InvalidExpr

};

class MemoryBlock
{
public:
    MemoryBlock(int value,int realValue)
    {
        Value = value;
        this->realValue = realValue;
    }
    int Value;//MIPS虚拟机中的内存地址
    int realValue;//实际内存地址
};

class AssemblyResult
{
public:
    QString description;
    InstrCheckType state;
    int lineIndex;
};

class N_Assembler :public QObject //这个类把汇编代码文本翻译成机器码文本
{
    Q_OBJECT
public:
    explicit N_Assembler(QObject* parent = nullptr);
    QWidget *preEnd;
    QStringList Assemble(const QString& orig);//返回汇编后的二进制机器码字符串
    QString Deassemble(QStringList inputBinLines);//返回反汇编后的指令字符串

signals:
    void initComplete(QHash<QString,QString>);

private:
    QStringList configFiles;
    QHash<QString,QString> RegDictionary;
    QHash<QString,QString> InstrOpcodes;//操作码到指令名的映射，方便反汇编,键值opcode_function
    QHash<QString,QString> RTypeInstrs;
    QHash<QString,QString> ITypeInstrs;
    QHash<QString,QString> JTypeInstrs;
    QHash<QString,QString> CTypeInstrs;
    QHash<QString,QString> PseudoInstrs;
    QHash<QString,QString> LabelTable;//变量和标号名到地址的映射
    void readConfig();//读取指令集、寄存器的配置文件
    int getExpValue(const QString& expr,bool* ok);//表达式求值器
    QString preProcess(const QString& text);//预处理：去除注释，扫描标号，变量内存分配，去除格式指令
    QString instrProcess(const QString& line,AssemblyResult* resultState);//处理读进的单行字符串
    QString pseudoInstrProcess(const QString& line,AssemblyResult* resultState);
    InstrCheckType isInputInstrValid(const QString& readInstr,INSTR_Type *type);//已废弃，判断某条指令合法性（名称是否存在、参数个数）
    void errorMsg(const AssemblyResult& result);//编译错误提示
    void showSheets();//调试用
    int lineIndex = 0;
    int originAddress = 0;
    int PC = 0;//程序计数器
    QString AssemblerOutMsg;
    MemoryBlock MemoryAlloc(int size);//待补充 size单位是zjie 16-bit
    bool isPreProcess = true;
    bool isNumString(const QString& str);
    int getNumStringValue(const QString& str);
    QHash<QString,QString> RegBinToName;

    QString RTypeDeassemble(const QString& input);
    QString ITypeDeassemble(const QString& input);
    QString JTypeDeassemble(const QString& input);

};



#endif // N_ASSEMBLER_H
