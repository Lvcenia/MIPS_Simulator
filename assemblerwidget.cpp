#include "assemblerwidget.h"
#include "ui_assemblerwidget.h"

#include "QDebug"
AssemblerWidget::AssemblerWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AssemblerWidget)
{
    ui->setupUi(this);
    this->assembler = new N_Assembler();
    assembler->preEnd = this;
    this->deassembler = new N_Deassembler();
    initConnection();
    setWindowIcon(parent->windowIcon());
    displayMode = HEX;
    connect(ui->textEdit_A_IN,&QTextEdit::textChanged,[=](){
        emit CodeTextChanged(ui->textEdit_A_IN->toPlainText());
    });
}

AssemblerWidget::~AssemblerWidget()
{
    delete ui;
}

N_Assembler *AssemblerWidget::GetAssembler()
{
    return this->assembler;
}

void AssemblerWidget::setVMInstance(MIPS_VM *vm)
{
    this->vmInstance = vm;
}


void AssemblerWidget::on_actionOpen_Triggered()
{
    QString curPath= QDir::currentPath();
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开文件"),curPath,"*.mips");
    QFile *file = new QFile(fileName);

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
         QMessageBox::warning(this,"打开失败",fileName + " 打开失败");
         return;
    }
    ui->tabWidget->setCurrentIndex(0);

    QTextStream aStream(file); //用文本流读取文件
    aStream.setCodec(QTextCodec::codecForName("utf-8"));
    ui->textEdit_A_IN->clear();//清空
    QString a = aStream.readAll();
    ui->textEdit_A_IN->setText(a);
    this->setWindowTitle(fileName);
    file->close();//关闭文件
    delete file;
}

void AssemblerWidget::on_actionSave_Triggered()
{
        QString curPath= QDir::currentPath();
    QString fileName = QFileDialog::getSaveFileName(this,"保存文本",curPath,"*.mips");

    QFile saveFile(fileName);
    if(!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
         QMessageBox::warning(this,"保存失败",fileName + " 保存失败");
         return;
    }
    else
    {
        QTextStream textStream(&saveFile);
        QString content = ui->textEdit_A_IN->toPlainText();
        textStream << content;
        saveFile.close();
    }

}

void AssemblerWidget::on_actionExport_Triggered()
{
    QString curPath= QDir::currentPath();
    QString fileName = QFileDialog::getSaveFileName(this,"导出二进制",curPath,"*.bin");
    QFile *file = new QFile(fileName);

    if (!file->open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
         QMessageBox::warning(this,"打开失败",fileName + " 打开失败");
         return;
    }

    QDataStream aStream(file); //用文本流写文件

    QStringList texts = outputText;

    bool ok;
    for(int i = 0;i < texts.count();i++)
    {
        qDebug()<<"写入：" << texts[i];
        if(!texts.at(i).isEmpty())
        {
            quint32 d = texts.at(i).toUInt(&ok,2);
            aStream << d;
        }

    }

    this->setWindowTitle(fileName);
    file->close();//关闭文件
    delete file;

}

void AssemblerWidget::on_actionOpenData_Triggered(bool isBigHead)
{
    inputBinText.clear();
    QString curPath= QDir::currentPath();
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开文件"),curPath,"*.bin");
    QFile *file = new QFile(fileName);

    if (!file->open(QIODevice::ReadOnly | QIODevice::Truncate))
    {
         QMessageBox::warning(this,"打开失败",fileName + " 打开失败");
         return;
    }
    ui->tabWidget->setCurrentIndex(1);

    QDataStream aStream(file); //用数据流读取文件

    ui->textEdit_DA_IN->clear();//清空
    quint32 num;
    quint8 bytes[4];
    int add = 0;
    int cnt = 0;
    int consecZeroes = 0;
    quint32 temp1,temp2;
    QList<quint32> dataSeg;
    QList<quint32> textSeg;
    bool isText = false;
    if(isBigHead)
    {
        while (!file->atEnd())
        {
         aStream >> num;
         if(!isText)
         {
             dataSeg.push_back(num);
             this->vmInstance->Memory->SetWord(add,num);
         }

         if(num == 0x00000000)
         {
             if(!isText)
             {
                 if(cnt >2)
                 {
                     if(dataSeg[cnt-1] == dataSeg[cnt-2] == 0x00000000)
                     {
                        isText = true;
                        add = 0;
                     }
                 }

             }
         }
         if(isText)
         {
             textSeg.push_back(num);
             this->vmInstance->Memory->SetInstr(add,num);
             inputBinText += QString("%1").arg(num,32,2,QChar('0'));
         }
         add+=2;
         cnt++;
        }
    }
    else
    {
        while (!file->atEnd())
        {
            //正着读 倒着算 例如 读出的byteArr是20 48 4b 01 重组出01 4b 48 20
            for(int i = 0; i < 4; i++)
            {
                aStream >> bytes[i];
                //qDebug()<<bytes[i];
            }
            num = bytes[3]<<24 | bytes[2]<<16 | bytes[1]<<8 | bytes[0];
            if(!isText)
            {
                dataSeg.push_back(num);
                //qDebug()<<dataSeg;
                this->vmInstance->Memory->SetWord(add,num);
            }

            if(num == 0x00000000)
            {
                if(!isText)
                {
                    if(cnt >2)
                    {
                        if(dataSeg[cnt-1] == dataSeg[cnt-2] == 0x00000000)
                        {
                           isText = true;
                           add = 0;
                        }
                    }

                }
            }
            qDebug()<<tr("%1").arg(num,8,16,QChar('0'));

            if(isText)
            {
                if(num == 0x00000000)
                    continue;
                textSeg.push_back(num);
                this->vmInstance->Memory->SetInstr(add,num);
                inputBinText += QString("%1").arg(num,32,2,QChar('0'));
            }
         add+=2;
         cnt++;
        }
    }


    this->setWindowTitle(fileName);
    file->close();//关闭文件
    delete file;
    rearrangeResultFormat(displayMode,ui->textEdit_DA_IN,inputBinText);
    QString dea = this->assembler->Deassemble(inputBinText);
    ui->textEdit_DA_OUT->setText(dea);
    emit DeassembleDataComplete(dea);
}



void AssemblerWidget::initConnection()
{
    connect(ui->actionOpen,&QAction::triggered,this,&AssemblerWidget::on_actionOpen_Triggered);
    connect(ui->actionExport,&QAction::triggered,this,&AssemblerWidget::on_actionExport_Triggered);
    connect(ui->actionOpenData,&QAction::triggered,this,&AssemblerWidget::on_actionOpenData_Triggered);
    connect(ui->actionSave,&QAction::triggered,this,&AssemblerWidget::on_actionSave_Triggered);

    connect(ui->actionExec,&QAction::triggered,[=](){

        switch (ui->tabWidget->currentIndex()) {
        case 0:
            qDebug() << "EXECUTING ASSEMBLY";
            this->outputText = this->assembler->Assemble(ui->textEdit_A_IN->toPlainText());
            rearrangeResultFormat(displayMode,ui->textEdit_A_OUT,this->outputText);

            break;
        case 1:break;
        default: break;
        }
    });


    this->connect(ui->actionBIN,&QAction::triggered,[=]()
    {
        displayMode = BIN;
        switch (ui->tabWidget->currentIndex())
        {
            case 0:rearrangeResultFormat(displayMode,ui->textEdit_A_OUT,this->outputText); break;
            case 1: rearrangeResultFormat(displayMode,ui->textEdit_DA_IN,this->inputBinText); break;
        }
        ui->labelModeDA->setText("显示模式：二进制");
        ui->labelMode->setText("显示模式：二进制");
        ui->actionBIN->setChecked(true);
        ui->actionHEX->setChecked(false);
    });
    this->connect(ui->actionHEX,&QAction::triggered,[=]()
    {
        displayMode = HEX;
        switch (ui->tabWidget->currentIndex())
        {
            case 0:rearrangeResultFormat(displayMode,ui->textEdit_A_OUT,this->outputText); break;
            case 1: rearrangeResultFormat(displayMode,ui->textEdit_DA_IN,this->inputBinText); break;
        }
                ui->labelModeDA->setText("显示模式：十六进制");
        ui->labelMode->setText("显示模式：十六进制");
        ui->actionBIN->setChecked(false);
        ui->actionHEX->setChecked(true);
    });

}

void AssemblerWidget::rearrangeResultFormat(DisplayMode mode,QTextEdit* textEdit,QStringList content)
{
    textEdit->clear();
    QStringList arranged(content);
    switch (mode)
    {
    case BIN:
        foreach(QString i,arranged)
        {
            textEdit->append(i);
        }
        break;
    case HEX:
        foreach(QString i,arranged)
        {
            if(i.isEmpty()) continue;

            bool ok;
            int64_t bin = i.toUInt(&ok,2);
            qDebug()<< ok;
            QString hex = QString::number(bin,16);
            while (hex.length()!=8) {
                hex.prepend('0');
            }

            i = "0x" + hex.toUpper();
            textEdit->append(i);
        }

        break;
    }

}


