#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <QMainWindow>
#include "n_assembler.h"
#include "assemblerwidget.h"


#include <QIcon>
#include <QStandardItemModel>
#include "mips_vm.h"

namespace Ui {
class Assembler;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void OnAsseblerWidgetTextChanged(const QString& curText);
    void OnMemoryDataChanged(int address,uint16_t prevValue,uint16_t curValue);
    void OnRegisterValueChanged(int number,QString name,int value);
    void OnCurrentInstrSetChanged(const QString& curInstrs);
    void OnCurrentInstrLineChanged(int line);

private:
    QStringList curInstrSet;
    Ui::Assembler *ui;
    void setTheme();
    void setMIPSRunningTheme();
    QString currentInstrs;
    AssemblerWidget* assemblerWidget;
    N_Assembler* assembler;
    QStandardItemModel *RegsTableModel;
    QStandardItemModel *MemoryTableModel;
    QStandardItemModel *InstrsTableModel;
    MIPS_VM* MIPS_VirtualMachine;
signals:
    void currentInstrSetChanged(const QString& curInstrS);

};

#endif // ASSEMBLER_H
