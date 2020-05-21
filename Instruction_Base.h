#ifndef INSTRUCTION_BASE_H
#define INSTRUCTION_BASE_H

#include <QObject>
#include <QHash>
enum INSTR_Type
{
    RType,
    IType,
    JType,
    CType,
    Pseudo,
    UNDEFINED

};

class Instruction_Base
{
public:
    Instruction_Base();
    virtual ~Instruction_Base(){}
    INSTR_Type Type;
    QString opCode;
    QString Name;
    QHash<QString,QString> params;
    virtual void paramInit()
    {
    }
    virtual QString Assemble()
    {
        return "";
    }

};

class RType_Instr : public Instruction_Base
{
public:RType_Instr(const QString& name)
    {
        this->Type = RType;
        this->opCode = "000000";
        this->Name = name;
        paramInit();
    }
    virtual void paramInit() override
    {
        this->params.insert("opCode","000000");
        this->params.insert("Func","000000");
        this->params.insert("rs","00000");
        this->params.insert("rt","00000");
        this->params.insert("rd","00000");
        this->params.insert("shamt","00000");
    }

    virtual QString Assemble() override;




};

class IType_Instr : public Instruction_Base
{
public:IType_Instr(const QString& opcode,const QString& name)
    {
        this->Type = IType;
        this->opCode = opcode;
        this->Name = name;
        paramInit();
    }
        virtual QString Assemble() override;
    virtual void paramInit() override
    {
        this->params.insert("opCode",opCode);
        this->params.insert("rs","00000");
        this->params.insert("rt","00000");
        this->params.insert("immediate","0000000000000000");
    }

};

class JType_Instr : public Instruction_Base
{
public:JType_Instr(const QString& opcode,const QString& name)
    {
        this->Type = JType;
        this->opCode = opcode;
        this->Name = name;
        paramInit();
    }
        virtual QString Assemble() override;
    virtual void paramInit() override
    {
        this->params.insert("opCode","000010");
        this->params.insert("label","00000000000000000000000000");
    }
};

class CType_Instr : public Instruction_Base
{
public:CType_Instr(const QString& name)
    {
        this->Type = CType;
        //this->opCode = opcode;
        this->Name = name;
        paramInit();
    }
        virtual QString Assemble() override;
    virtual void paramInit() override
    {
        this->params.insert("opCode","010000");
        this->params.insert("Func","000000");
        this->params.insert("format","000000");
        this->params.insert("fs","00000");
        this->params.insert("ft","00000");
        this->params.insert("fd","00000");
    }

};



#endif // INSTRUCTION_BASE_H
