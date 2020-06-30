#include "vmcpu.h"

vmCPU::vmCPU(QObject *parent) : QObject(parent)
{
    initRegDict();
    initInstrSet();



}

void vmCPU::ExecInstruction(const QString &Instr)
{
    if(Instr.trimmed().isEmpty())
        return;
    QStringList SlicedInstr = Instr.split(" ");
    SlicedInstr[0] = SlicedInstr.at(0).toUpper();
    QStringList params;
    if(!SlicedInstr[1].isNull())
         params = SlicedInstr[1].split(QRegExp("[,()]"),QString::SplitBehavior::SkipEmptyParts);
    switch(InstrStringToEnum[SlicedInstr[0]])
    {
    case LUi:
    {
        QString rt = params[0];
        int dat = params[1].toInt(nullptr,16);
          RegistersValue[RegDict[rt]] = dat<<16;
          emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
          PC+=2;
    }

        break;
    case ADD:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] + RegistersValue[RegDict[rt]];
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }
        break;
    case ADDi:
    {
        int dat = params[2].toInt(nullptr,16);
        QString rt = params[0];
        QString rs = params[1];
        RegistersValue[RegDict[rt]] = RegistersValue[RegDict[rs]] + dat;
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
        PC+=2;
    }
        break;
    case ADDiu:
    {
        qDebug() << "in addiu";
        uint32_t dat = params[2].toUInt(nullptr,16);
        QString rt = params[0];
        QString rs = params[1];

        RegistersValue[RegDict[rt]] = RegistersValue[RegDict[rs]] + dat;
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
                PC+=2;
    }
        break;
    case SUB:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] < RegistersValue[RegDict[rt]] ? 1:0;
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }break;
    case SLT:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] + RegistersValue[RegDict[rt]];
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }break;
    case SLTi:
    {
        int dat = params[2].toInt(nullptr,16);
        QString rt = params[0];
        QString rs = params[1];
        RegistersValue[RegDict[rt]] = RegistersValue[RegDict[rs]] < dat ? 1:0;
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
        PC+=2;
    }
        break;
    case SLTu:
    {

        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = ((long)RegistersValue[RegDict[rs]] & 0xffffffffl) < ((long)RegistersValue[RegDict[rt]] & 0xffffffffl) ? 1:0;
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
        PC+=2;
    }break;
    case SLTiu:
    {
        int dat = params[2].toInt(nullptr,16);
        QString rt = params[0];
        QString rs = params[1];
        RegistersValue[RegDict[rt]] = ((long)RegistersValue[RegDict[rs]] & 0xffffffffl) < ((long)dat & 0xffffffffl) ? 1:0;
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
        PC+=2;
    }
        break;
    case AND:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] & RegistersValue[RegDict[rt]];
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }
        break;
    case ANDi:
    {
        int dat = params[2].toInt(nullptr,16);
        QString rt = params[0];
        QString rs = params[1];
        RegistersValue[RegDict[rt]] = RegistersValue[RegDict[rs]] & dat;
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
        PC+=2;
    }
        break;
    case OR:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] | RegistersValue[RegDict[rt]];
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }break;
    case ORi:
    {
        int dat = params[2].toInt(nullptr,16);
        QString rt = params[0];
        QString rs = params[1];
        RegistersValue[RegDict[rt]] = RegistersValue[RegDict[rs]] | dat;
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
        PC+=2;
    }break;
    case XOR:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] ^ RegistersValue[RegDict[rt]];
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }break;
    case XORi:
    {
        int dat = params[2].toInt(nullptr,16);
        QString rt = params[0];
        QString rs = params[1];
        RegistersValue[RegDict[rt]] = RegistersValue[RegDict[rs]] ^ dat;
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
        PC+=2;
    }break;
    case NOR:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = ~(RegistersValue[RegDict[rs]] | RegistersValue[RegDict[rt]]);
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }break;
    case SLL:
    {
        QString rd = params[0];
        QString rs = params[1];
        int sa = params[2].toInt(nullptr,16);
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] << sa;
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }break;
    case SLLv:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] <<  RegistersValue[RegDict[rt]];
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }break;
    case SRL:    {
        QString rd = params[0];
        QString rs = params[1];
        int sa = params[2].toInt(nullptr,16);
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] >> sa;
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }break;
    case SRLv:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] >>  RegistersValue[RegDict[rt]];
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }break;
    case SRA:
    {
            QString rd = params[0];
            QString rs = params[1];
            int sa = params[2].toInt(nullptr,16);
            RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] >> sa;
            emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
            PC+=2;
        }break;
    case SRAv:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] >>  RegistersValue[RegDict[rt]];
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }break;
    case LW:
    {
        short dat = params[1].toShort(nullptr,16);
        QString rt = params[0];
        QString rs = params[2];
        qDebug()<< "in lw " << toHex(RegistersValue[RegDict[rs]]+dat);
        uint32_t address = 0;
        if(this->currentMode == ZPC)
        {
            address = RegistersValue[RegDict[rs]]+dat;
        }
        else//标准MIPS
        {
            uint32_t base = STATIC_DATA_SEG_START + (RegistersValue[RegDict[rs]] - STATIC_DATA_SEG_START)/2;
            uint32_t ofs = dat/2;
            address = base + ofs;
        }
        RegistersValue[RegDict[rt]] = this->mem->GetWordValue(address);
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
        PC+=2;
    }
        break;
    case LH:
    {
        short dat = params[1].toShort(nullptr,16);
        QString rt = params[0];
        QString rs = params[2];
        qDebug()<< "in lw " << toHex(RegistersValue[RegDict[rs]]+dat);
        uint32_t address = 0;
        if(this->currentMode == ZPC)
        {
            address = RegistersValue[RegDict[rs]]+dat;
        }
        else//标准MIPS
        {
            uint32_t base = STATIC_DATA_SEG_START + (RegistersValue[RegDict[rs]] - STATIC_DATA_SEG_START)/2;
            uint32_t ofs = dat/2;
            address = base + ofs;
        }
        RegistersValue[RegDict[rt]] = this->mem->GetValue(address);
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
        PC+=2;
    }break;
    case LHu:
    {
        short dat = params[1].toShort(nullptr,16);
        QString rt = params[0];
        QString rs = params[2];
        qDebug()<< "in lw " << toHex(RegistersValue[RegDict[rs]]+dat);
        uint32_t address = 0;
        if(this->currentMode == ZPC)
        {
            address = RegistersValue[RegDict[rs]]+dat;
        }
        else//标准MIPS
        {
            uint32_t base = STATIC_DATA_SEG_START + (RegistersValue[RegDict[rs]] - STATIC_DATA_SEG_START)/2;
            uint32_t ofs = dat/2;
            address = base + ofs;
        }
        RegistersValue[RegDict[rt]] = (uint32_t)this->mem->GetValue(address);
        emit registerValueChanged(RegDict[rt],rt,RegistersValue[RegDict[rt]]);
        PC+=2;
    }break;
    case SW:
    {
        short dat = params[1].toShort(nullptr,16);
        QString rt = params[0];
        QString rs = params[2];
        this->mem->SetWord(RegistersValue[RegDict[rs]]+dat,RegistersValue[RegDict[rt]]);
        //emit registerValueChanged("$" +RegDict[rt],rt,RegistersValue[RegDict[rt]]);
        PC+=2;
    }
        break;
    case SH:
    {
        short dat = params[1].toShort(nullptr,16);
        QString rt = params[0];
        QString rs = params[2];
        this->mem->SetZjie(RegistersValue[RegDict[rs]]+dat,RegistersValue[RegDict[rt]]);
        //emit registerValueChanged("$" +RegDict[rt],rt,RegistersValue[RegDict[rt]]);
        PC+=2;
    }break;
    case BEQ:
    {
        QString rs = params[0];
        QString rt = params[1];
        QString ofs = params[2];
        if(RegistersValue[RegDict[rs]] == RegistersValue[RegDict[rt]])
            this->PC+=2* ofs.toInt();
        else
            PC+=2;
    }break;
    case BNE:
    {
        QString rs = params[0];
        QString rt = params[1];
        QString ofs = params[2];
        if(RegistersValue[RegDict[rs]] != RegistersValue[RegDict[rt]])
            this->PC+=2 *ofs.toInt();
        else
            PC+=2;
    }break;
    case BGEZAL:
    {
        QString rs = params[0];
        QString ofs = params[1];
        if(RegistersValue[RegDict[rs]] >= 0)
        {
            RegistersValue[RegDict["$ra"]] = PC;
            this->PC+=2*ofs.toInt();
        }
        else
            PC+=2;

    }break;
    case J://伪直接寻址
    {
        QString adr = params[0];
        this->PC = (PC&0xF8000000)|adr.toInt();

    }break;
    case JAL://伪直接寻址
    {
        QString adr = params[0];
        RegistersValue[RegDict["$ra"]] = PC;
        this->PC = (PC&0xF8000000) + adr.toInt();

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
    case MFCO:break;
    case MTCO:break;
    case ERET:break;
    case SYSCALL:
        this->SysCallProcess(RegistersValue[RegDict["$v0"]]);
        PC+=2;
        break;
    case MUL:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] =(uint32_t)(RegistersValue[RegDict[rs]] * RegistersValue[RegDict[rt]]);
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }break;
    case MULT:
    {
        QString rs = params[0];
        QString rt = params[1];
        this->hi  = (int)(((long)(RegistersValue[RegDict[rs]] * RegistersValue[RegDict[rt]]) >>32)& 0xffffffff);
        this->lo = (int)(((long)(RegistersValue[RegDict[rs]] * RegistersValue[RegDict[rt]]))& 0xffffffff);

        emit registerValueChanged(RegDict[rs],rs,RegistersValue[RegDict[rs]]);
        PC+=2;
    }break;
    case DIV:
    {
        QString rs = params[0];
        QString rt = params[1];
        this->lo  = RegistersValue[RegDict[rs]] / RegistersValue[RegDict[rt]];
        this->hi = RegistersValue[RegDict[rs]] % RegistersValue[RegDict[rt]];

        emit registerValueChanged(RegDict[rs],rs,RegistersValue[RegDict[rs]]);
        PC+=2;
    }break;
    case DIVu:
    {
        QString rs = params[0];
        QString rt = params[1];
        this->lo  = RegistersValue[RegDict[rs]] / RegistersValue[RegDict[rt]];
        this->hi = RegistersValue[RegDict[rs]] % RegistersValue[RegDict[rt]];

        emit registerValueChanged(RegDict[rs],rs,RegistersValue[RegDict[rs]]);
        PC+=2;
    }break;
    case MFHi:
    {
        QString rd = params[0];

        RegistersValue[RegDict[rd]] = this->hi;

        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }break;
    case MTHi:
    {
        QString rs = params[0];

        this->hi = RegistersValue[RegDict[rs]];

        //emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }break;
    case MTLO:
    {
        QString rd = params[0];

        RegistersValue[RegDict[rd]] = this->lo;

        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }break;
    case SUBu:
    {
        QString rd = params[0];
        QString rs = params[1];
        QString rt = params[2];
        RegistersValue[RegDict[rd]] = RegistersValue[RegDict[rs]] < RegistersValue[RegDict[rt]] ? 1:0;
        emit registerValueChanged(RegDict[rd],rd,RegistersValue[RegDict[rd]]);
        PC+=2;
    }break;
    default:break;
    }

}

void vmCPU::setMemory(vmMemory *m)
{
    this->mem = m;
}

void vmCPU::setFrontEnd(QWidget *front)
{
    this->frontend = front;
}

void vmCPU::SetInstrMode(InstrMode mode)
{
    this->currentMode = mode;
}

void vmCPU::initInstrSet()
{
    InstrStringToEnum.insert("LUI",LUi);
    InstrStringToEnum.insert("ADD",ADD);
    InstrStringToEnum.insert("ADDI",ADDi);
    InstrStringToEnum.insert("ADDIU",ADDiu);
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
    InstrStringToEnum.insert("MFCO",MFCO);
    InstrStringToEnum.insert("MTCO",MTCO);
    InstrStringToEnum.insert("ERET",ERET);
    InstrStringToEnum.insert("SYSCALL",SYSCALL);
    InstrStringToEnum.insert("MUL",MUL);

    InstrStringToEnum.insert("MULT",MULT);
    InstrStringToEnum.insert("DIV",DIV);
    InstrStringToEnum.insert("DIVu",DIVu);
    InstrStringToEnum.insert("MFHi",MFHi);
    InstrStringToEnum.insert("MLFO",MLFO);
    InstrStringToEnum.insert("MTHI",MTHi);
    InstrStringToEnum.insert("MTLO",MTLO);
    InstrStringToEnum.insert("SUBU",SUBu);
    InstrStringToEnum.insert("BGEZ",BGEZ);
    InstrStringToEnum.insert("BGTZ",BGTZ);
    InstrStringToEnum.insert("BLEZ",BLEZ);
    InstrStringToEnum.insert("BLTZ",BLTZ);
    InstrStringToEnum.insert("BLTZAL",BLTZAL);

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
    QVariant value;
    switch (code) {
    case 1://print int
        SystemPrint<int>(RegistersValue[RegDict["$a0"]]);
        break;
    case 2://print float
        SystemPrint<float>(RegistersValue[RegDict["$f12"]]);
        break;
    case 3://print double
        SystemPrint<double>(RegistersValue[RegDict["$f12"]]);
        break;
    case 4://print string
    {
        QString str;
        QByteArray data;
        int add = RegistersValue[RegDict["$a0"]];
        while(1)
        {
            uint16_t zjie = this->mem->GetValue(add);
            if(zjie == 0) break;
            char b1,b2;
            b1 = (char)((zjie & 0xff00) >> 2);
            b2 = (char)(zjie & 0x00ff);
            data.push_back(b1);
            data.push_back(b2);
            add++;
        }
        str = str.fromLocal8Bit(data);
        SystemPrint<QString>(str);
    }

        break;
    case 5://read int
        value = GetSystemInput();
        RegistersValue[RegDict["$a0"]] = value.toInt();
        break;
    case 6://read float
        value = GetSystemInput();
        RegistersValue[RegDict["$f0"]] = value.toFloat();
        break;
    case 7://read double
        value = GetSystemInput();
        RegistersValue[RegDict["$f0"]] = value.toDouble();
        break;
    case 8://read string
        {
            value = GetSystemInput();
            RegistersValue[RegDict["$a0"]] = StoreInputString(value.toString());
        }
        break;
    case 9://sbrk
        break;
    case 10://exit
        emit ProcessEnded();
        break;
    case 11://print char
         SystemPrint<char>(RegistersValue[RegDict["$a0"]]);
        break;
    case 12://read char
        value = GetSystemInput();
        RegistersValue[RegDict["$a0"]] = StoreInputString(value.toString());
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

QString vmCPU::GetSystemInput()
{
    bool ok;
    QString input = QInputDialog::getText(this->frontend, tr("System Input"),
                                          tr("System Input :"), QLineEdit::Normal,
                                          tr("请输入"), &ok);
    if(!ok) return "";
    return input;

}

uint32_t vmCPU::StoreInputString(const QString &str)
{
    QTextCodec* gb2312 = QTextCodec::codecForName("GB2312");
    QString a = gb2312->toUnicode(str.toLocal8Bit());
    QByteArray u = a.toLocal8Bit();
    QList<uint16_t> string_Zjie;
    for(int i = 0; i < u.size();i+=2)
    {
        uint16_t zjie = (((uint16_t)u[i]) << 8) |  (uint16_t)u[i+1];
        string_Zjie.push_back(zjie);
    }

}

QString vmCPU::toHex(int num)
{
    return QString("0x%1").arg(num,8,16,QChar('0'));

}



void vmCPU::OnStartExcecution(QString instrs)
{
    PC = TEXT_SEGMENT_START;
    initRegVals();
    if(instrs.isEmpty()) return;
    emit ProcessStarted();
    isRunning = true;
    int i = TEXT_SEGMENT_START;
    for(auto instr:instrs.split("\n"))
    {
        this->curInstrSet.insert(i,instr);
        i+=2;
    }
    ProcessEndAddress = i;
    this->ExecInstruction(curInstrSet[PC]);
    emit currentInstrChanged(PC);

}

void vmCPU::OnStepToNext()
{
    if(PC==this->ProcessEndAddress)
    {
        OnProcessEnd();
        return;
    }
    this->ExecInstruction(curInstrSet[PC]);
    emit currentInstrChanged(PC);

}

void vmCPU::OnProcessEnd()
{
    isRunning = false;
    emit ProcessEnded();
    initRegVals();
    return;

}
