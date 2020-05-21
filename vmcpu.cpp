#include "vmcpu.h"

vmCPU::vmCPU(QObject *parent) : QObject(parent)
{
    initRegDict();
    initInstrSet();



}

void vmCPU::ExecInstruction(const QString &Instr)
{
    QStringList SlicedInstr = Instr.trimmed().split(" ");
    SlicedInstr[0] = SlicedInstr.at(0).toUpper();
    QStringList params = SlicedInstr[1].split(QRegExp("[,()]"),QString::SplitBehavior::SkipEmptyParts);
    switch(InstrStringToEnum[SlicedInstr[0]])
    {
    case LUi:
    {
        QString rt = params[0];
        int dat = params[1].toInt();
          RegistersValue[RegDict[rt]] = dat<<16;
          emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
    }

        break;
    case ADD:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] + RegistersValue[RegDict[rt]];
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
    }
        break;
    case ADDi:
    {
        int dat = params[2].toInt();
        QString rt = params[0];
        QString rs = params[1];
        RegistersValue[RegDict[rt]] = RegistersValue[RegDict[rs]] + dat;
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
    }
        break;
    case SUB:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] < RegistersValue[RegDict[rt]] ? 1:0;
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
    }break;
    case SLT:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] + RegistersValue[RegDict[rt]];
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
    }break;
    case SLTi:
    {
        int dat = params[2].toInt();
        QString rt = params[0];
        QString rs = params[1];
        RegistersValue[RegDict[rt]] = RegistersValue[RegDict[rs]] < dat ? 1:0;
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
    }
        break;
    case SLTu:break;
    case SLTiu:break;
    case AND:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] & RegistersValue[RegDict[rt]];
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
    }
        break;
    case ANDi:
    {
        int dat = params[2].toInt();
        QString rt = params[0];
        QString rs = params[1];
        RegistersValue[RegDict[rt]] = RegistersValue[RegDict[rs]] & dat;
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
    }
        break;
    case OR:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] | RegistersValue[RegDict[rt]];
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
    }break;
    case ORi:
    {
        int dat = params[2].toInt();
        QString rt = params[0];
        QString rs = params[1];
        RegistersValue[RegDict[rt]] = RegistersValue[RegDict[rs]] | dat;
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
    }break;
    case XOR:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] ^ RegistersValue[RegDict[rt]];
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
    }break;
    case XORi:
    {
        int dat = params[2].toInt();
        QString rt = params[0];
        QString rs = params[1];
        RegistersValue[RegDict[rt]] = RegistersValue[RegDict[rs]] ^ dat;
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
    }break;
    case NOR:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = ~(RegistersValue[RegDict[rs]] | RegistersValue[RegDict[rt]]);
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
    }break;
    case SLL:
    {
        QString rd = params[0];
        QString rs = params[1];
        int sa = params[2].toInt();
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] << sa;
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
    }break;
    case SLLv:break;
    case SRL:break;
    case SRLv:break;
    case SRA:break;
    case SRAv:break;
    case LW:
    {
        short dat = params[1].toShort();
        QString rt = params[0];
        QString rs = params[2];
        RegistersValue[RegDict[rt]] = this->mem->GetValue(RegistersValue[RegDict[rs]]+dat);
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
    }
        break;
    case LWx:break;
    case LH:break;
    case LHx:break;
    case LHu:break;
    case LHux:break;
    case SW:
    {
        short dat = params[1].toShort();
        QString rt = params[0];
        QString rs = params[2];
        this->mem->SetValue(RegistersValue[RegDict[rs]]+dat,RegistersValue[RegDict[rt]]);
        //emit registerValueChanged("$" +RegDict[rt],rt,RegistersValue[RegDict[rt]]);
    }
        break;
    case SWx:break;
    case SH:break;
    case SHx:break;
    case BEQ:
    {
        QString rs = params[0];
        QString rt = params[1];
        QString ofs = params[2];
        if(RegistersValue[RegDict[rs]] == RegistersValue[RegDict[rt]])
            this->PC+=ofs.toInt();
    }break;
    case BNE:
    {
        QString rs = params[0];
        QString rt = params[1];
        QString ofs = params[2];
        if(RegistersValue[RegDict[rs]] != RegistersValue[RegDict[rt]])
            this->PC+=ofs.toInt();
    }break;
    case BGEZAL:
    {
        QString rs = params[0];
        QString ofs = params[1];
        if(RegistersValue[RegDict[rs]] >= 0)
        {
            RegistersValue[RegDict["$ra"]] = PC;
            this->PC+=ofs.toInt();
        }

    }break;
    case J:
    {
        QString adr = params[0];
        this->PC = (PC&0xF8000000)|adr.toInt();

    }break;
    case JAL:
    {
        QString adr = params[0];
        RegistersValue[RegDict["$ra"]] = PC;
        this->PC = (PC&0xF8000000)|adr.toInt();

    }break;
    case JR:
    {
        QString rs = params[0];
         this->PC = RegistersValue[RegDict[rs]];

    }break;
    case JALr:
    {
        QString rs = params[0];
        QString rd = params[1];
        RegistersValue[RegDict[rd]] = PC;
        PC = RegistersValue[RegDict[rs]];

    }break;
    case MFC0:break;
    case MTC0:break;
    case ERET:break;
    case SYSCALL:
        this->SysCallProcess(RegistersValue[RegDict["$v0"]]);
        break;
    default:break;
    }

}

void vmCPU::setMemory(vmMemory *m)
{
    this->mem = m;
}

void vmCPU::initInstrSet()
{
    InstrStringToEnum.insert("LUI",LUi);
    InstrStringToEnum.insert("ADD",ADD);
    InstrStringToEnum.insert("ADDI",ADDi);
    InstrStringToEnum.insert("SUB",SUB);
    InstrStringToEnum.insert("SLT",SLT);
    InstrStringToEnum.insert("SLTI",SLTi);
    InstrStringToEnum.insert("SLTU",SLTu);
    InstrStringToEnum.insert("SLTIU",SLTiu);
    InstrStringToEnum.insert("AND",AND);
    InstrStringToEnum.insert("ANDI",ANDi);
    InstrStringToEnum.insert("OR",OR);
    InstrStringToEnum.insert("ORI",ORi);
    InstrStringToEnum.insert("XOR",XOR);
    InstrStringToEnum.insert("XORI",XORi);
    InstrStringToEnum.insert("NOR",NOR);
    InstrStringToEnum.insert("SLL",SLL);
    InstrStringToEnum.insert("SLLV",SLLv);
    InstrStringToEnum.insert("SRL",SRL);
    InstrStringToEnum.insert("SRLV",SRLv);
    InstrStringToEnum.insert("SRA",SRA);
    InstrStringToEnum.insert("SRAV",SRAv);
    InstrStringToEnum.insert("LW",LW);
    InstrStringToEnum.insert("LWX",LWx);
    InstrStringToEnum.insert("LH",LH);
    InstrStringToEnum.insert("LHX",LHx);
    InstrStringToEnum.insert("LHU",LHu);
    InstrStringToEnum.insert("LHUX",LHux);
    InstrStringToEnum.insert("SW",SW);
    InstrStringToEnum.insert("SWX",SWx);
    InstrStringToEnum.insert("SH",SH);
    InstrStringToEnum.insert("SHX",SHx);
    InstrStringToEnum.insert("BEQ",BEQ);
    InstrStringToEnum.insert("BNE",BNE);
    InstrStringToEnum.insert("BGEZAL",BGEZAL);
    InstrStringToEnum.insert("J",J);
    InstrStringToEnum.insert("JAL",JAL);
    InstrStringToEnum.insert("JR",JR);
    InstrStringToEnum.insert("JALR",JALr);
    InstrStringToEnum.insert("MFC0",MFC0);
    InstrStringToEnum.insert("MTC0",MTC0);
    InstrStringToEnum.insert("ERET",ERET);
    InstrStringToEnum.insert("SYSCALL",SYSCALL);

}

void vmCPU::initRegDict()
{
            RegDict.insert("$zero",0);
            RegDict.insert("$at",1);
            RegDict.insert("$v0",2);
            RegDict.insert("$v1",3);
            RegDict.insert("$a0",4);
            RegDict.insert("$a1",5);
            RegDict.insert("$a2",6);
            RegDict.insert("$a3",7);
            RegDict.insert("$t0",8);
            RegDict.insert("$t1",9);
            RegDict.insert("$t2",10);
            RegDict.insert("$t3",11);
            RegDict.insert("$t4",12);
            RegDict.insert("$t5",13);
            RegDict.insert("$t6",14);
            RegDict.insert("$t7",15);
            RegDict.insert("$s0",16);
            RegDict.insert("$s1",17);
            RegDict.insert("$s2",18);
            RegDict.insert("$s3",19);
            RegDict.insert("$s4",20);
            RegDict.insert("$s5",21);
            RegDict.insert("$s6",22);
            RegDict.insert("$s7",23);
            RegDict.insert("$t8",24);
            RegDict.insert("$t9",25);
            RegDict.insert("$k0",26);
            RegDict.insert("$k1",27);
            RegDict.insert("$gp",28);
            RegDict.insert("$sp",29);
            RegDict.insert("$fp",30);
            RegDict.insert("$ra",31);

}

void vmCPU::initRegVals()
{
    QHash<QString,int>::iterator i;
    for (i = RegDict.begin(); i != RegDict.end(); ++i) {
        //qDebug()<< "$" + QString::number(i.value())<<"+"<<i.key();
        emit registerValueChanged(i.value(),i.key(),0);
    }

}

void vmCPU::SysCallProcess(int code)
{
    switch (code) {
    case 1://print int
        break;
    case 2://print float
        break;
    case 3://print double
        break;
    case 4://print string
        break;
    case 5://read int
        break;
    case 6://read float
        break;
    case 7://read double
        break;
    case 8://read string
        break;
    case 9://sbrk
        break;
    case 10://exit
        break;
    case 11://print char
        break;
    case 12://read char
        break;
    case 13://open
        break;
    case 14://read
        break;
    case 15://write
        break;    
    case 16://close
        break;
    case 17://exit2
        break;

    default:
        break;
    }

}

void vmCPU::OnStartExcecution(QString instrs)
{
    PC = 0;
    initRegVals();
    if(instrs.isEmpty()) return;
    emit ProcessStarted();
    isRunning = true;
    this->curInstrSet = instrs.split("\n");
    qDebug()<<curInstrSet[PC];
    this->ExecInstruction(curInstrSet[PC]);
    PC++;
    emit currentInstrChanged(PC);

}

void vmCPU::OnStepToNext()
{
    if(PC>=this->curInstrSet.length())
    {
        emit ProcessEnded();
        initRegVals();
        return;
    }
    this->ExecInstruction(curInstrSet[PC]);
    PC++;
    emit currentInstrChanged(PC);

}
