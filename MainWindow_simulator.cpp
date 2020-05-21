#include "MainWindow_simulator.h"
#include "ui_MIPS_simulator.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Assembler)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("://style/MIPS_icon.png"));
    setTheme();
    this->assemblerWidget = new AssemblerWidget(this);
    this->assembler = assemblerWidget->GetAssembler();

    ui->textEditInstrs->hide();
    this->RegsTableModel = new QStandardItemModel(32,3,this);//编号 名字 值
    this->MIPS_VirtualMachine = new MIPS_VM(this);
    this->assemblerWidget->setVMInstance(this->MIPS_VirtualMachine);
    connect(ui->actionOpenAssembler,&QAction::triggered,[=](){
        this->assemblerWidget->show();
    });
    //connect(this->RegsTableModel,&QStandardItemModel::dataChanged,ui->tableViewRegs,&QTableView::dataChanged);
    QStringList labels = QObject::trUtf8("编号,名字,值").simplified().split(",");
        RegsTableModel->setHorizontalHeaderLabels(labels);
    ui->tableViewRegs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewRegs->verticalHeader()->setVisible(false);
    ui->tableViewRegs->setModel(this->RegsTableModel);




    this->MemoryTableModel = new QStandardItemModel(0,2,this);//地址 值
    //connect(this->MemoryTableModel,&QStandardItemModel::dataChanged,ui->tableViewMemory,&QTableView::dataChanged);
       QStringList labels2 = QObject::trUtf8("地址,值").simplified().split(",");
       MemoryTableModel->setHorizontalHeaderLabels(labels2);
    ui->tableViewMemory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewMemory->verticalHeader()->setVisible(false);
    ui->tableViewMemory->setModel(this->MemoryTableModel);

    this->InstrsTableModel = new QStandardItemModel(0,2,this);//指令名，参数表
     ui->tableViewInstrs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QStringList labels3 = QObject::trUtf8("指令,参数").simplified().split(",");
    InstrsTableModel->setHorizontalHeaderLabels(labels3);
    ui->tableViewInstrs->setModel(InstrsTableModel);


    connect(this->assemblerWidget,&AssemblerWidget::CodeTextChanged,this,&MainWindow::OnAsseblerWidgetTextChanged);

    connect(ui->actionSysExec,&QAction::triggered,[=](){
        ui->tableViewInstrs->selectRow(0);
        this->OnCurrentInstrSetChanged(this->currentInstrs);
        this->MIPS_VirtualMachine->CPU->OnStartExcecution(this->currentInstrs);
    });

    connect(ui->actionStepInto,&QAction::triggered,[=](){
       this->MIPS_VirtualMachine->CPU->OnStepToNext();
    });

    connect(ui->actionCollabInput,&QAction::triggered,[=](){
        if(ui->actionCollabInput->isChecked())
        {
            connect(assemblerWidget,&AssemblerWidget::CodeTextChanged,this,&MainWindow::OnAsseblerWidgetTextChanged);
        }
        else
        {
            disconnect(assemblerWidget,&AssemblerWidget::CodeTextChanged,this,&MainWindow::OnAsseblerWidgetTextChanged);
        }
    });


    connect(ui->textEditInstrs,&QTextEdit::textChanged,[=](){
        //this->currentInstrs = ui->textEditInstrs->toPlainText();
            //emit currentInstrSetChanged(this->currentInstrs);
    });

    connect(ui->actionOpenBinary,&QAction::triggered,this->assemblerWidget,&AssemblerWidget::on_actionOpenData_Triggered);

    connect(this->assemblerWidget,&AssemblerWidget::DeassembleDataComplete,this,&MainWindow::OnCurrentInstrSetChanged);

    connect(this->MIPS_VirtualMachine->CPU,&vmCPU::registerValueChanged,this,&MainWindow::OnRegisterValueChanged);
    connect(this->MIPS_VirtualMachine->Memory,&vmMemory::valueChanged,this,&MainWindow::OnMemoryDataChanged);


    connect(this->MIPS_VirtualMachine->CPU,&vmCPU::currentInstrChanged,this,&MainWindow::OnCurrentInstrLineChanged);

    connect(this->MIPS_VirtualMachine->CPU,&vmCPU::ProcessStarted,[=](){
        qDebug()<<"started";
        this->setMIPSRunningTheme();
    });
    connect(this->MIPS_VirtualMachine->CPU,&vmCPU::ProcessEnded,[=](){
        qDebug()<<"ended";
        this->setTheme();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnAsseblerWidgetTextChanged(const QString &curText)
{
    ui->textEditInstrs->setText(curText);
        this->currentInstrs = curText;
    emit currentInstrSetChanged(curText);
    }

void MainWindow::OnMemoryDataChanged(int address, uint16_t prevValue, uint16_t curValue)
{
        QStandardItem* addressItem = new QStandardItem(QString::number(address,16));
        QStandardItem* curValItem = new QStandardItem("0x" + QString::number(curValue,16).toUpper());
        QList<QStandardItem*> x;
        x<<addressItem;
        x<<curValItem;
        this->MemoryTableModel->insertRow(0,x);


}

void MainWindow::OnRegisterValueChanged(int number,QString name,int value)
{

    QStandardItem* regVal = new QStandardItem("0x" + QString::number(value,16).toUpper());
    QStandardItem* regName = new QStandardItem(name);
    QStandardItem* regNumName = new QStandardItem("$" + QString::number(number,10));
    this->RegsTableModel->setItem(number,0,regNumName);
    this->RegsTableModel->setItem(number,1,regName);
    this->RegsTableModel->setItem(number,2,regVal);


}

void MainWindow::OnCurrentInstrSetChanged(const QString &curInstrs)
{
    if(curInstrs.trimmed().isEmpty()) return;
    this->currentInstrs = curInstrs;
    this->InstrsTableModel->clear();

    this->curInstrSet = curInstrs.split("\n");
    QStringList labels3 = QObject::trUtf8("指令,参数").simplified().split(",");
    InstrsTableModel->setHorizontalHeaderLabels(labels3);
    for(int i = 0; i < curInstrSet.length();i++)
    {
        if(curInstrSet[i].trimmed().isEmpty()) continue;
        QStandardItem* instrName = new QStandardItem(curInstrSet[i].split(" ").first());
        QStandardItem* params = new QStandardItem(curInstrSet[i].split(" ").last());
        QList<QStandardItem*> x;
        x<<instrName;
        x<<params;
        this->InstrsTableModel->insertRow(i,x);
    }

}

void MainWindow::OnCurrentInstrLineChanged(int line)
{
        ui->tableViewInstrs->selectRow(line);

}

void MainWindow::setTheme()
{

        QFile styleSheet(":/config/Config/style/dark.qss");
        if (styleSheet.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream aStream(&styleSheet); //用文本流读取文件
            this->setStyleSheet(aStream.readAll());
            styleSheet.close();//关闭文件
        }
        ui->statusBar->setStyleSheet("QStatusBar{background-color:rgb(64,66,68);}");
}

void MainWindow::setMIPSRunningTheme()
{
//    QFile styleSheet(":/config/Config/style/dark.qss");
//    if (styleSheet.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        QTextStream aStream(&styleSheet); //用文本流读取文件
//        this->setStyleSheet(aStream.readAll());
//        styleSheet.close();//关闭文件
//    }
    ui->statusBar->setStyleSheet("QStatusBar{background-color:rgb(255,154,65);}");



    //ui->centralWidget->setStyleSheet("QWidget{background-color:rgb(255,154,65);}");
}


