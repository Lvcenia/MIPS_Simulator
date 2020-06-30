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
    this->displayWidget = new DisplayWidget(this);
    displayWidget->hide();
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




    this->MemoryTableModel_Text = new QStandardItemModel(0,9,this);//类似mars 8列
    this->MemoryTableModel_Data = new QStandardItemModel(0,9,this);
    //connect(this->MemoryTableModel,&QStandardItemModel::dataChanged,ui->tableViewMemory_Text,&QTableView::dataChanged);
       QStringList labels2 = QObject::trUtf8("地址,值(+0),值(+1),值(+2),值(+3),值(+4),值(+5),值(+6),值(+7)").simplified().split(",");
       QStringList labelsText = QObject::trUtf8("地址,值(+0),值(+2),值(+4),值(+6),值(+8),值(+a),值(+c),值(+f)").simplified().split(",");
       MemoryTableModel_Text->setHorizontalHeaderLabels(labelsText);
       MemoryTableModel_Data->setHorizontalHeaderLabels(labels2);

    ui->tableViewMemory_Text->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewMemory_Text->verticalHeader()->setVisible(true);
    ui->tableViewMemory_Text->setModel(this->MemoryTableModel_Text);

    ui->tableViewMemory_Data->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewMemory_Data->verticalHeader()->setVisible(true);
    ui->tableViewMemory_Data->setModel(this->MemoryTableModel_Data);

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

    connect(ui->actionOpenDisplay,&QAction::triggered,[=](){
       displayWidget->show();
    });



    connect(ui->textEditInstrs,&QTextEdit::textChanged,[=](){
        //this->currentInstrs = ui->textEditInstrs->toPlainText();
            //emit currentInstrSetChanged(this->currentInstrs);
    });

    connect(ui->actionOpenBinary,&QAction::triggered,this->assemblerWidget,&AssemblerWidget::on_actionOpenData_Triggered);

    connect(this->assemblerWidget,&AssemblerWidget::DeassembleDataComplete,this,&MainWindow::OnCurrentInstrSetChanged);

    connect(this->MIPS_VirtualMachine->CPU,&vmCPU::registerValueChanged,this,&MainWindow::OnRegisterValueChanged);
    connect(this->MIPS_VirtualMachine->Memory,&vmMemory::valueChanged,this,&MainWindow::OnMemoryDataChanged);
    connect(this->MIPS_VirtualMachine->Memory,&vmMemory::instrChanged,this,&MainWindow::OnMemoryTextChanged);

    connect(this->MIPS_VirtualMachine->CPU,&vmCPU::currentInstrChanged,this,&MainWindow::OnCurrentInstrLineChanged);

    connect(this->MIPS_VirtualMachine->CPU,&vmCPU::ProcessStarted,[=](){
        ui->textEditOUT->clear();
        ui->textEditOUT->append("Process Started");
        qDebug()<<"started";
        this->setMIPSRunningTheme();
    });
    connect(this->MIPS_VirtualMachine->CPU,&vmCPU::ProcessEnded,[=](){
        ui->textEditOUT->append("Process Ended");
        qDebug()<<"ended";
        this->setTheme();
    });

    connect(this->MIPS_VirtualMachine->CPU,&vmCPU::sysCallResultGot,[=](const QString& text){
       ui->textEditOUT->append(text);
    });

    connect(ui->actionZPC,&QAction::triggered,[=](){
        ui->actionZPC->setChecked(true);
        ui->actionStandard_MIPS->setChecked(false);
        this->MIPS_VirtualMachine->CPU->SetInstrMode(ZPC);
    });

    connect(ui->actionStandard_MIPS,&QAction::triggered,[=](){
        ui->actionZPC->setChecked(false);
        ui->actionStandard_MIPS->setChecked(true);
        this->MIPS_VirtualMachine->CPU->SetInstrMode(StandardMIPS);
    });

    initMemoryTables();


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
        int row = address/8;
        int column = 1+address%8;

        //QStandardItem* addressItem = new QStandardItem(QString::number(address,16));
        QStandardItem* curValItem = new QStandardItem(tr("0x%1").arg(curValue,4,16,QChar('0')));
        //if(address%8 ==0)
          //  this->MemoryTableModel_Data->setItem(row,0,addressItem);
        this->MemoryTableModel_Data->setItem(row,column,curValItem);
        ui->tableViewMemory_Data->update();
        //this->MemoryTableModel_Text->insertRow(0,x);
}

void MainWindow::OnMemoryTextChanged(int address, uint32_t prevValue, uint32_t curValue)
{
    qDebug()<<"in text changed  " << address << "  " << curValue;

    int row = address/16;
    int column = 1+(address/2)%8;

    //QStandardItem* addressItem = new QStandardItem(QString::number(address,16));
    QStandardItem* curValItem = new QStandardItem(tr("0x%1").arg(curValue,8,16,QChar('0')));
    //if(address%8 ==0)
       // this->MemoryTableModel_Text->setItem(row,0,addressItem);
    this->MemoryTableModel_Text->setItem(row,column,curValItem);
    ui->tableViewMemory_Text->update();
    //this->MemoryTableModel_Text->insertRow(0,x);


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
    QStringList labels3 = QObject::trUtf8("地址,指令,参数").simplified().split(",");
    InstrsTableModel->setHorizontalHeaderLabels(labels3);
    int address = TEXT_SEGMENT_START;
    for(int i = 0; i < curInstrSet.length();i++)
    {
        if(curInstrSet[i].trimmed().isEmpty()) continue;
        QStandardItem* addressItem = new QStandardItem(tr("0x%1").arg(address,8,16,QChar('0')));
        QStandardItem* instrName = new QStandardItem(curInstrSet[i].split(" ").first());
        QStandardItem* params = new QStandardItem(curInstrSet[i].split(" ").last());
        QList<QStandardItem*> x;
        x<<addressItem;
        x<<instrName;
        x<<params;
        this->InstrsTableModel->insertRow(i,x);
        address+=2;
    }

}

void MainWindow::OnCurrentInstrLineChanged(int PC)
{
    ui->labelPC->setText(QString("PC: 0x%1").arg(PC,8,16,QChar('0')));
    int line = (PC-TEXT_SEGMENT_START)/2;
    ui->tableViewInstrs->selectRow(line);

}


void MainWindow::setTheme()
{
    qApp->setStyle(QStyleFactory::create("Fusion"));
        QPalette palette;
        palette.setColor(QPalette::Window, QColor(53,53,53));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(15,15,15));
        palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(53,53,53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Highlight, QColor(68,45,236).lighter());
        palette.setColor(QPalette::HighlightedText, Qt::black);
        qApp->setPalette(palette);

//        QFile styleSheet(":/config/Config/style/dark.qss");
//        if (styleSheet.open(QIODevice::ReadOnly | QIODevice::Text))
//        {
//            QTextStream aStream(&styleSheet); //用文本流读取文件
//            this->setStyleSheet(aStream.readAll());
//            styleSheet.close();//关闭文件
//        }
//        ui->statusBar->setStyleSheet("QStatusBar{background-color:rgb(64,66,68);}");
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

void MainWindow::initMemoryTables()
{
    int cnt = 0;
    for(auto i = TEXT_SEGMENT_START;i< TEXT_SEGMENT_START + 4096 ;i+=16)
    {

        QString initRow = tr("0x%1,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000");
        //initRow = initRow.arg(i,8,16,QChar('0'));
        QStringList x = initRow.split(',');
        x[0] = x[0].arg(i,8,16,QChar('0'));
        QList<QStandardItem*> s;
        for(auto item:x)
        {
            QStandardItem* si = new QStandardItem(item);
            s.push_back(si);
        }
        this->MemoryTableModel_Text->insertRow(cnt,s);
        cnt++;
    }

    cnt = 0;
    for(auto i = STATIC_DATA_SEG_START;i< STATIC_DATA_SEG_START + 4096;i+=8)
    {
        QString initRow = "0x%1,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000";
        //initRow = initRow.arg(i,8,16,QChar('0'));
        QStringList x = initRow.split(',');
        x[0] = x[0].arg(i,8,16,QChar('0'));
        QList<QStandardItem*> s;
        for(auto item:x)
        {
            QStandardItem* si = new QStandardItem(item);
            s.push_back(si);
        }
        this->MemoryTableModel_Data->insertRow(cnt,s);
        cnt++;
    }

}


