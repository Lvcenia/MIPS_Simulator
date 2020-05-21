#include "n_assembler.h"

N_Assembler::N_Assembler(QObject* parent)
{
    this->configFiles += ":/config/Config/Registers_0.txt";
    this->configFiles += ":/config/Config/Instructions_Pseudo.txt";
    this->configFiles += ":/config/Config/Instructions_R.txt";
    this->configFiles += ":/config/Config/Instructions_I.txt";
    this->configFiles += ":/config/Config/Instructions_J.txt";
    this->configFiles += ":/config/Config/Instructions_C.txt";
    readConfig();
    emit initComplete(this->RegDictionary);
}



void N_Assembler::readConfig()//读取配置文件
{
    QFile file;
    qDebug()<<"Reading Config Files";
    QString configFile;
    QString line;
    foreach(configFile,configFiles)
    {
        //qDebug()<<"Currently reading : " <<configFile;

        if(configFile.isEmpty())
        {
            qDebug()<< "Config Not Found";
            return;
        }

        file.setFileName(configFile);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
             qDebug()<<"打开文件失败";
             continue;
        }
        QTextStream aStream(&file); //用文本流读取文件
        aStream.setCodec(QTextCodec::codecForName("utf-8"));
        char type = configFile.split("_").last().at(0).toLatin1();
        line = aStream.readLine();
        switch (type)
        {
        case '0':
            //qDebug()<< "Reading Regs";
            while(!line.isNull())
            {
                if(line.at(0) != '#')
                {
                    this->RegDictionary.insert(line.split(" ").first(),line);
                    this->RegBinToName.insert(line.split(" ").last(),line.split(" ").first());
                }

                line = aStream.readLine();
                //qDebug()<<"a: " << line;

            }
            break;
        case 'R':
           //qDebug()<< "Reading RInstr";
            while(!line.isNull())
            {
                if(line.at(0) != '#')
                {
                    this->RTypeInstrs.insert(line.split("|").first(),line);
                    this->InstrOpcodes.insert("0_" + line.split("|")[2],line.split("|").first());
                }

                line = aStream.readLine();
                //qDebug()<<"a: " << line;
            }
            break;
        case 'I':
            while(!line.isNull())
            {
                if(line.at(0) != '#')
                {
                    this->ITypeInstrs.insert(line.split("|").first(),line);
                    this->InstrOpcodes.insert(line.split("|")[2],line.split("|").first());
                    //qDebug()<< "Reading IInstr";

                    //qDebug()<<"a: " << line.split("|")[2];
                }
                   line = aStream.readLine();


            }
            break;
        case 'J':
                //qDebug()<< "Reading JInstr";
            while(!line.isNull())
            {
                if(line.at(0) != '#')
                {
                    this->JTypeInstrs.insert(line.split("|").first(),line);
                    this->InstrOpcodes.insert(line.split("|")[2],line.split("|").first());
                }

                line = aStream.readLine();
                //qDebug()<<"a: " << line;

            }
            break;
        case 'C':
                 //qDebug()<< "Reading CInstr";
            while(!line.isNull())
            {
                if(line.at(0) != '#')
                this->CTypeInstrs.insert(line.split("|").first(),line);
                line = aStream.readLine();
                //qDebug()<<"a: " << line;

            }
            break;
        case 'P':
                //qDebug()<< "Reading PseudoInstr";
            while(!line.isNull())
            {
                if(line.at(0) != '#')
                this->PseudoInstrs.insert(line.split("|").first(),line);
                line = aStream.readLine();
            }
            break;
        default: break;
        }
        file.close();//关闭文件
    }
    //showSheets();
}


QString N_Assembler::instrProcess(const QString &line,AssemblyResult* result)
{
    QString assemblyResult;//指令汇编的结果字符串
    assemblyResult = line.simplified();//去除头尾空格和中间多余空格
    qDebug()<<"input Line "<< assemblyResult;
    (*result).state = Correct;
    if(assemblyResult == "") return assemblyResult;//如果是空行 返回
    bool isInstrValid = false;
    QStringList splittedInput = assemblyResult.split(" ");
    QString name = splittedInput.first().toLower();
    QStringList Realparams;//读入指令的实参表


    if(isPreProcess)//预处理的时候展开伪指令
    {
        if(this->PseudoInstrs.contains(name))
        {
            isInstrValid = true;
            qDebug() << assemblyResult << " is Pseudo Instr";
            QStringList format = PseudoInstrs[name].split("|");
            //format[0] 指令名 [1]参数表 [2]参数个数 [3]真实指令
            Realparams = splittedInput.last().split(QRegExp("[,()]"),QString::SplitBehavior::SkipEmptyParts);
            if(!(name == "push" || name == "pop") && Realparams.count() != format[2].toInt())//参数个数检查
            {
                   (*result).state = WrongParamNum;
                   (*result).description = "指令 " + name + " " + QString("提供 %1个，需要 %2个").arg(Realparams.count()).arg(format.last().toInt());
            }
            else
            {
                QStringList formatParams = format[1].split(QRegExp("[,()]"),QString::SplitBehavior::SkipEmptyParts);
                QString realMachineCode = format[3].replace("_","\n");
                QString result;
                if(name == "push" || name == "pop") //push pop参数不固定 特别处理
               {
                    int n = Realparams.count();

                    QString addi = realMachineCode.split("\n").first();
                    addi = addi.replace("n*2",QString::number(n*2)) + "\n";
                    QString sw;
                    for(int i = 0; i < n; i ++)
                    {
                        qDebug() << i << " " + realMachineCode.split("\n").last().replace("%i",Realparams[i]).replace("2*i",QString::number(2*i));
                        sw += realMachineCode.split("\n").last().replace("%i",Realparams[i]).replace("2*i",QString::number(2*i)) + "\n";
                    }
                    if(name == "push") assemblyResult = addi + sw;
                    else assemblyResult = sw + addi;
               }
                else if(name == "li" || name == "la")
                {

                }

               else//其他固定参数的伪指令
               {
                   for(int i = 0; i < formatParams.count();i++)
                   {
                        assemblyResult = realMachineCode.replace(formatParams[i],Realparams[i]);
                   }
               }
                qDebug() <<"Result : " <<assemblyResult;
                return assemblyResult;
            }

        }
    }
    else//正式编译
    {
        if(this->RTypeInstrs.contains(name))//R类型处理
        {
            isInstrValid = true;
            qDebug() << assemblyResult << " is RType";
            QStringList format = RTypeInstrs[name].split("|");
            //format[0]是指令名 [1]是参数表 [2]是功能值 [3]是参数个数
            Realparams = splittedInput.last().split(",");
            if(!(name == "break" || name == "syscall") && Realparams.count() != format.last().toInt())
            {
                 (*result).state = WrongParamNum;
                 (*result).description = "指令 " + name + " " +QString("提供 %1个，需要 %2个").arg(Realparams.count()).arg(format.last().toInt());
            }
            else
            {
                RType_Instr instr(name);
                QStringList formatParams = format[1].split(",");
                //整理标准格式的字段，formatParams的内容为类似{"rs","rt","rd"}
                instr.params["Func"] = format[2];//设置功能码
                if(name == "break" || name == "syscall")
                    ;
                else
                {
                    for(int i = 0; i <formatParams.count(); i++)
                    {
                        qDebug()<<"format: " << formatParams[i];
                        qDebug()<<"real: " << instr.params[formatParams[i]];
                        if(RegDictionary.contains(Realparams[i]))//如果写的直接就是寄存器
                        {
                            instr.params[formatParams[i]] = RegDictionary[Realparams[i]].split(" ").last();//这里进行变量（寄存器）的数值替换，如果没有某一项，会被置0
                        }
                        else if(LabelTable.contains(Realparams[i]))//如果是标号
                        {
                            //QMessageBox::information(preEnd,"标号",QString(LabelTable[Realparams[i]]));

                        }
                        else//不是标准寄存器，也不是标号
                        {
                            (*result).state = UndefinedLabel;
                            (*result).description = Realparams[i];
                        }
                    }
                }
            assemblyResult = instr.Assemble();//得到汇编后的二进制指令字符串
             }
        }

        if(this->ITypeInstrs.contains(name))//I类型处理
        {
            isInstrValid = true;
            qDebug() << assemblyResult << " is IType";
            QStringList format = ITypeInstrs[name].split("|");
            qDebug()<< format;
            //format[0]是指令名 [1]是参数表 [2]是操作码 [3]是参数个数
            Realparams = splittedInput.last().split(QRegExp("[,()]"),QString::SplitBehavior::SkipEmptyParts);
            if(Realparams.count() != format.last().toInt())//参数个数检查
            {
                (*result).state = WrongParamNum;
               (*result).description = "指令 " + name + " " + QString("提供 %1个，需要 %2个").arg(Realparams.count()).arg(format.last().toInt());
            }
            else
            {
                IType_Instr instr(format[2],name);

                QStringList formatParams = format[1].split(QRegExp("[,()]"),QString::SplitBehavior::SkipEmptyParts);
                qDebug()<<formatParams;
                //整理标准格式的字段，formatParams的内容为类似{"rs","immediate(r)"}
            for(int i = 0; i <formatParams.count(); i++)
            {
                qDebug()<<"format: " << formatParams[i];
                if(RegDictionary.contains(Realparams[i]))//如果写的直接就是寄存器
                {
                instr.params[formatParams[i]] = RegDictionary[Realparams[i]].split(" ").last();//这里进行变量（寄存器）的数值替换，如果没有某一项，会被置0
                }
                if(formatParams[i] == "label")
                {
                    bool ok;
                    int value;
                    //用表达式求值器检查立即数合法性
                    QString expr = Realparams[i];
                    if(LabelTable.contains(Realparams[i]))//如果是标号
                    {
                        expr = LabelTable[Realparams[i]];
                    }
                    value = getExpValue(expr,&ok);
                    QString imeResult = "%1";
                    if(value >= 0) imeResult = imeResult.arg(value,26,2,QChar('0'));
                    else imeResult = imeResult.arg(value,26,2).mid(64-26);
                    if(ok)
                    {
                        instr.params["label"] = imeResult;
                    }
                    else
                    {
                        result->state = InvalidExpr;
                        result->description = "不合法表达式 " + Realparams[i];
                    }

                }
                if(formatParams[i] == "immediate")//如果是立即数
                {
                    bool ok = true;
                    int value = getExpValue(Realparams[i],&ok);
                    QString imeResult = "%1";
                    if(value >= 0) imeResult = imeResult.arg(value,16,2,QChar('0'));
                    else imeResult = imeResult.arg(value,26,2).mid(64-16);
                    //用表达式求值器检查立即数合法性
                    if(ok)
                    {
                        instr.params["immediate"] = imeResult;
                    }
                    else
                    {
                        result->state = InvalidExpr;
                        result->description = "不合法表达式 " + Realparams[i];
                    }

                }
                qDebug()<<"real: " << instr.params[formatParams[i]];
            }
            if(name == "bgez") instr.params["rt"] = "00001";
            assemblyResult = instr.Assemble();//得到汇编后的二进制指令字符串
             }

        }

        if(this->JTypeInstrs.contains(name))//因为J类就两条，所以没用上面那套
        {
            isInstrValid = true;
            qDebug() << assemblyResult << " is JType";
            QStringList format = JTypeInstrs[name].split("|");
            qDebug()<< format;
            //format[0]是指令名 [1]是参数表 [2]是操作码 [3]是参数个数
            Realparams = splittedInput.last().split(QRegExp("[,()]"),QString::SplitBehavior::SkipEmptyParts);
            if(Realparams.count() != format.last().toInt())//参数个数检查
            {
                (*result).state = WrongParamNum;
               (*result).description ="指令 " + name + " " + QString("提供 %1个，需要 %2个").arg(Realparams.count()).arg(format.last().toInt());
            }
            else
            {
                JType_Instr instr(format[2],name);
                QStringList formatParams = format[1].split(QRegExp("[,()]"),QString::SplitBehavior::SkipEmptyParts);
                qDebug()<<formatParams;
                //整理标准格式的字段，formatParams的内容为{"label"}
            for(int i = 0; i <formatParams.count(); i++)
            {
                qDebug()<<"format: " << formatParams[i];
//                if(RegDictionary.contains(Realparams[i]))//如果写的直接就是寄存器
//                {
//                instr.params[formatParams[i]] = RegDictionary[Realparams[i]].split(" ").last();//这里进行变量（寄存器）的数值替换，如果没有某一项，会被置0
//                }
                if(formatParams[i] == "label")//如果是标号
                {
                    bool ok;
                    int value;
                    //用表达式求值器检查立即数合法性
                    QString expr = Realparams[i];
                    if(LabelTable.contains(Realparams[i]))//如果是标号
                    {
                        expr = LabelTable[Realparams[i]];
                    }
                    value = getExpValue(expr,&ok);
                    QString imeResult = "%1";
                    if(value >= 0) imeResult = imeResult.arg(value,26,2,QChar('0'));
                    else imeResult = imeResult.arg(value,26,2).mid(64-26);
                    if(ok)
                    {                     
                        instr.params["label"] = imeResult;
                    }
                    else
                    {
                        result->state = InvalidExpr;
                        result->description = "不合法表达式 " + Realparams[i];
                    }

                }
                qDebug()<<"real: " << instr.params[formatParams[i]];
            }
            assemblyResult = instr.Assemble();//得到汇编后的二进制指令字符串
             }
        }

        if(this->CTypeInstrs.contains(name))
        {
            isInstrValid = true;
            qDebug() << assemblyResult << " is CType";
            QStringList format = CTypeInstrs[name].split("|");
            qDebug()<< format;
            //format[0]是指令名 [1]是参数表 [2]是功能码 [3]是格式码 [4]是参数个数
            Realparams = splittedInput.last().split(QRegExp("[,()]"),QString::SplitBehavior::SkipEmptyParts);
            if(Realparams.count() != format.last().toInt())//参数个数检查
            {
                (*result).state = WrongParamNum;
               (*result).description ="指令 " + name + " " + QString("提供 %1个，需要 %2个").arg(Realparams.count()).arg(format.last().toInt());
            }
            else
            {
                CType_Instr instr(name);//操作码统一都是010000

                QStringList formatParams = format[1].split(QRegExp("[,()]"),QString::SplitBehavior::SkipEmptyParts);
                qDebug()<<formatParams;
                //整理标准格式的字段
            for(int i = 0; i <formatParams.count(); i++)
            {
                qDebug()<<"format: " << formatParams[i];
                if(RegDictionary.contains(Realparams[i]))//如果写的直接就是寄存器
                {
                instr.params[formatParams[i]] = RegDictionary[Realparams[i]].split(" ").last();//这里进行变量（寄存器）的数值替换，如果没有某一项，会被置0
                }

                qDebug()<<"real: " << instr.params[formatParams[i]];
            }
                instr.params["format"] = format[3];
                instr.params["Func"] = format[2];
                assemblyResult = instr.Assemble();//得到汇编后的二进制指令字符串
             }
        }

        if(!isInstrValid)
        {
            (*result).state = WrongInstrName;
            (*result).description = name;
        }
    }

    switch ((*result).state)//根据汇编状态确定返回值
    {
    case Correct:
        break;
    case WrongInstrName:
        return "Err_WrongInstrName";
    case WrongParamNum:
        return "Err_WrongParamNum";
    case UndefinedLabel:
        return "Err_UndefinedLabel";
    case InvalidExpr:
        return "Err_InvalidExpr";
    }

    qDebug() <<"Assemble Result: " << assemblyResult;
    return assemblyResult;
}



void N_Assembler::errorMsg(const AssemblyResult& result)
{
    QString ErrMsg = QString("Error : 在第%1行\n").arg(result.lineIndex);
    switch (result.state)
    {
    case Correct:break;
    case WrongParamNum:
        AssemblerOutMsg += ErrMsg + "提供的参数个数与允许的参数个数不一致\n"  + result.description  + "\n";
        break;
    case WrongInstrName:
        AssemblerOutMsg += ErrMsg + QString("提供的指令名称 %1 不存在或未被支持\n").arg(result.description)  +  "\n";
        break;
    case UndefinedLabel:
        AssemblerOutMsg += ErrMsg  + QString("使用了未声明的变量或标号‘%1’\n").arg(result.description)  + "\n";
        break;
    case InvalidExpr:
        AssemblerOutMsg += ErrMsg  + QString("非法表达式‘%1’\n").arg(result.description)  + "\n";
        break;
    }
}

void N_Assembler::showSheets()
{
    qDebug() <<"Reg: " <<this->RegDictionary;
    qDebug() <<"R: "<< this->RTypeInstrs;
    qDebug() <<"I: "<< this->ITypeInstrs;
    qDebug() <<"J: "<< this->JTypeInstrs;
    qDebug() <<"C: "<< this->CTypeInstrs;
}

MemoryBlock N_Assembler::MemoryAlloc(int size)
{

}

bool N_Assembler::isNumString(const QString &str)
{
    QString s(str);
    if(s.startsWith("-")) s = s.mid(1);
    if(s.startsWith("0x") ||s.startsWith("0b"))
    s = s.replace("0x","").replace("0b","");

    for(int i = 0; i < s.count(); i++)
    {
        if(!s.at(i).isDigit()) return false;
    }
    return true;
}

int N_Assembler::getNumStringValue(const QString &str)
{
    int value = 1;
    int base = 10;
    QString s(str);
    if(s.startsWith("-"))
    {
        value *= -1;
        s = s.mid(1);
    }
    if(s.startsWith("0x"))
    {
        base = 16;

    }

    if(s.startsWith("0b"))
    {
        s = s.mid(2);
        base = 2;
    }
    value *= s.toInt(nullptr,base);
    return value;
}




QStringList N_Assembler::Assemble(const QString &orig)
{
    isPreProcess = true;
    bool isDataStarted = false;
    bool isTextStarted = false;
    AssemblerOutMsg.clear();
    QString line;
    qDebug() << "Before preprocess: " << orig;
    QStringList lines = preProcess(orig).split("\n");
    isPreProcess = false;
    qDebug() << "After preprocess: " << lines;
        lineIndex = 0;
        PC = originAddress;
    foreach (line, lines)//逐行扫描预处理以后的指令集
    {
        line = line.simplified();
        if(line.isEmpty())
        {
            lines.removeAt(lineIndex);
            continue;
        }
        lineIndex++;

        if(line.toLower() == ".data")//数据声明块
        {
            line.clear();
            isDataStarted = true;
            isTextStarted = false;
        }

        if(line.toLower() == ".text")//代码块
        {
            line.clear();
            isDataStarted = false;
            isTextStarted = true;
        }
        if(isTextStarted)
       {
            AssemblyResult rs;
            rs.lineIndex = lineIndex;
            line = instrProcess(line,&rs);//汇编指令转换
            errorMsg(rs);
            qDebug()<< "real Out: " << line << "res: " << rs.description;
       }
        lines[lineIndex-1] = line;
    }
    if(!this->AssemblerOutMsg.isEmpty())
        QMessageBox::warning(preEnd,"编译错误",AssemblerOutMsg);
    qDebug()<<lines;
    return lines;

}

QString N_Assembler::RTypeDeassemble(const QString &input)
{
    QString func = input.mid(26,6);
    QString rs = input.mid(6,5);
    QString rt = input.mid(11,5);
    QString rd = input.mid(16,5);
    QString res = "";
    res += this->InstrOpcodes["0_" + func];
    res += " ";
    res += this->RegBinToName[rd];
    res += ",";
    res += this->RegBinToName[rs];
    res += ",";
    res += this->RegBinToName[rt];
    return res;

}

QString N_Assembler::ITypeDeassemble(const QString &input)
{
    QString opcode = input.mid(0,6);
    QString rs = input.mid(6,5);
    QString rt = input.mid(11,5);
    QString dat = input.mid(16,16);
    QString res = "";
    if(this->InstrOpcodes[opcode].contains("sw") || this->InstrOpcodes[opcode].contains("lw"))
    {
        QString name = this->InstrOpcodes[opcode];
        if(name.contains("n_"))
        {
            name = name.mid(2);
        }
        res += name;
        res += " ";
        res += this->RegBinToName[rt];
        res += ",";
        res += QString::number(dat.toInt(nullptr,2));
        res += "(";
        res += this->RegBinToName[rs];
        res += ")";
    }
    else
    {
        res += this->InstrOpcodes[opcode];
        res += " ";
        res += this->RegBinToName[rt];
        res += ",";
        res += this->RegBinToName[rs];
        res += ",";
        res += QString::number(dat.toInt(nullptr,2));
    }

    return res;

}

QString N_Assembler::JTypeDeassemble(const QString &input)
{
    QString opcode = input.mid(0,6);
    QString target = input.mid(6,26);

    QString res = "";
    res += this->InstrOpcodes[opcode];
    res += " ";
    res += QString::number(target.toInt(nullptr,2));

    return res;

}



QString N_Assembler::Deassemble(QStringList inputBinLines)
{
    QStringList lines(inputBinLines);
    QString res  = "";
    foreach (QString i, lines) {//逐句反汇编
        QString opcode = i.mid(0,6);
        if(opcode == "000000")
        {
            i = RTypeDeassemble(i);
            res += i;
            qDebug()<<"da:"<<i;
        }
        else if(opcode.mid(0,5) == "00001")//J
        {
            i = JTypeDeassemble(i);
             res += i;
            qDebug()<<"da:"<<i;
        }
        else if (opcode.mid(0,4) == "0100")//C类型
        {

        }
        else //I类型
        {
            i = ITypeDeassemble(i);
                        res += i;
                        qDebug()<<"da:"<<i;
        }
        res += "\n";
    }
    return res;

}


QString N_Assembler::preProcess(const QString &text)
{
    lineIndex = 0;
    bool isDataStarted = false;
    bool isTextStarted = false;
    QStringList lines = text.split("\n");//分行
    QString result;
     //QRegExp(spt);
    foreach (QString line, lines)//逐行扫描
    {
        lineIndex++;
        line = line.split("#").first();//每一行去除注释
        line = line.simplified();
        if(line.toLower() == ".data")//数据声明块
        {
            isDataStarted = true;
            isTextStarted = false;
        }

        if(line.toLower() == ".text")//代码块
        {
            isDataStarted = false;
            isTextStarted = true;
        }

        if(line.toLower().contains(".origin"))//设置程序起始位置
        {
            originAddress = line.simplified().split(" ").last().toInt();
            PC = originAddress;
            line.clear();
        }

        if(line.toLower().contains("equ"))
        {
            QStringList i = line.split(" ");
            if(i.count() == 3 && "equ" == i[1])
            {
                if(isNumString(i[2]))
                LabelTable.insert(i[0],i[2]);
            }
            line.clear();
        }

        if(isDataStarted)//对数据块的变量生成数据，记录内存地址
        {

            line = line.simplified();
            QStringList variableLine = line.split(" ");
            //variableLine[0] 变量名 [1]变量类型 [2]值
            if(variableLine.count() == 3)
            {
                if(variableLine[1].toLower() == ".word" || variableLine[1].toLower() == ".2zjie")
                {


                }
                else if (variableLine[1].toLower() == ".zjie")
                {

                }

                LabelTable.insert(variableLine[0],QString::number(PC));
            }
            PC+=2;


        }
        if(isTextStarted)//对代码段的预处理
        {
            AssemblyResult rs;
            if(line.contains(":") && line.split(":").first().isEmpty() == false)//记录标号所在行对应的地址，然后从行中去除标号
            {
                if(line.split(":").last().simplified().isEmpty())//标号单独一行
                    PC-=2;
                LabelTable.insert(line.split(":").first().simplified(),QString::number(PC));
                line = line.split(":").last();
            }
            line = instrProcess(line,&rs);//展开伪指令
            PC+=2;
            if(line.split("\n",QString::SkipEmptyParts).count() > 1)
            {
                PC += (line.split("\n",QString::SkipEmptyParts).count() - 1)*2;
            }
            errorMsg(rs);
        }
        lines[lineIndex-1] = line;
        result += line + "\n";
    }
    if(!this->AssemblerOutMsg.isEmpty())
        QMessageBox::warning(preEnd,"预编译错误",AssemblerOutMsg);
    qDebug() <<"Labels : " << LabelTable;

    return result;

}


int N_Assembler::getExpValue(const QString &expr,bool* ok)
{
    if(isNumString(expr))//如果表达式是纯数，直接返回值
    {
        (*ok) = true;
        return getNumStringValue(expr);
    }

    else
    {
        (*ok) = false;
            return 0;
    }


}
