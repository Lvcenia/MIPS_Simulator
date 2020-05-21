#-------------------------------------------------
#
# Project created by QtCreator 2020-04-27T21:45:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MIPS_Simulator_2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    assemblerwidget.cpp \
    Instruction_Base.cpp \
    MainWindow_simulator.cpp \
    mips_vm.cpp \
    n_assembler.cpp \
    n_deassembler.cpp \
    vmcpu.cpp \
    vmmemory.cpp


HEADERS += \
    assemblerwidget.h \
    Instruction_Base.h \
    MainWindow_simulator.h \
    mips_vm.h \
    n_assembler.h \
    n_deassembler.h \
    vmcpu.h \
    vmmemory.h


FORMS += \
        assemblerwidget.ui \
    MIPS_simulator.ui

RESOURCES += \
        config.qrc
RC_ICONS = MIPS_Simulator.ico
DISTFILES += \
    ../NotePadPlusPros/NAssembler/MIPS_Simulator.ico \
    MIPS_Simulator.ico
