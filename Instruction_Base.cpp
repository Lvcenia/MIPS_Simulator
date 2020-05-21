#include "Instruction_Base.h"

Instruction_Base::Instruction_Base()
{

}

QString RType_Instr::Assemble()
{
    QString result;
    result += this->params["opCode"];
    result += this->params["rs"];
    result += this->params["rt"];
    result += this->params["rd"];
    result += this->params["shamt"];
    result += this->params["Func"];
    return result;
}

QString IType_Instr::Assemble()
{
    QString result;
    result += this->params["opCode"];
    result += this->params["rs"];
    result += this->params["rt"];
    result += this->params["immediate"];
    return result;
}

QString JType_Instr::Assemble()
{
    QString result;
    result += this->params["opCode"];
    result += this->params["label"];
    return result;
}

QString CType_Instr::Assemble()
{
    QString result;
    result += this->params["opCode"];
    result += this->params["format"];
    result += this->params["ft"];
    result += this->params["fs"];
    result += this->params["fd"];
    result += this->params["Func"];
    return result;
}
