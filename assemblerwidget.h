#ifndef ASSEMBLERWIDGET_H
#define ASSEMBLERWIDGET_H

#include <QMainWindow>
#include "n_assembler.h"
#include "n_deassembler.h"
#include <QTextEdit>
#include <QFileDialog>
#include <QDir>
#include <QDataStream>
#include "mips_vm.h"


namespace Ui {
class AssemblerWidget;
}

enum AssemblerMode
{
    Assemble,
    Deassemble
};

enum DisplayMode
{
    BIN,
    HEX
};

class AssemblerWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit AssemblerWidget(QWidget *parent = 0);
    ~AssemblerWidget();
    N_Assembler* GetAssembler();
    void setVMInstance(MIPS_VM* vm);

signals:
    void CodeTextChanged(const QString& curText);
    void DeassembleDataComplete(const QString& deassembled);


public slots:
    void on_actionOpen_Triggered();
    void on_actionSave_Triggered();
    void on_actionExport_Triggered();
    void on_actionOpenData_Triggered();
    
private:
    MIPS_VM* vmInstance;
    AssemblerMode mode;
    DisplayMode displayMode;
    Ui::AssemblerWidget *ui;
    N_Assembler *assembler;
    N_Deassembler *deassembler;
    QStringList outputText;//二进制文本串
    QStringList inputBinText;
    void initConnection();
    void rearrangeResultFormat(DisplayMode mode,QTextEdit* textEdit,QStringList content);

};

#endif // ASSEMBLERWIDGET_H
