# MIPS_Simulator
MIPS_Simulator 是一个基于Qt实现的简单的MIPS模拟器，带有图形界面，可以进行MIPS代码的运行和调试，同时集成了MIPS汇编代码编辑、汇编和反汇编的功能。 

## 基础介绍

### 特性
- 支持 .asm 汇编文件汇编为 .bin 文件；
- 支持 .bin 文件反汇编为 .asm 汇编文件；
- 支持读入.bin文件后的模拟运行，支持模拟终端输入输出；
- 支持简单的调试功能，包括单步运行和全部运行、以及查看寄存器与内存的值，内存值会根据.text段和.data段进行区分。
- 实现了基于窗口绘制的图形化显示器

### 界面与操作
下图是程序的基础界面

模拟器窗口包括以下子窗口：（全部采用活动窗口，用户可以自由排列组合子窗口的位置，也可以将子窗口拖出）
- 指令：在读入.bin文件后显示所有指令的列表，在运行时，会高亮显示当前即将被执行的指令
- 寄存器：包含一个32行的表格，实时显示32个CPU寄存器的状态
- 内存：一个分页窗口，包含代码页和数据页，每一页包含一个9列的表格，实时显示内存的状态
- 输出：一个包含只读文本框的窗口，在运行时会输出程序的输出信息，同时，在需要输入时，会弹出对话框来接收用户输入
- 菜单栏：包含打开二进制、运行程序、单步运行程序、打开汇编器。

### 指令集
MIPS_Simulator支持以下指令的汇编、反汇编与模拟。

支持的 R 指令：
- add
- addu
- and
- div
- divu
- jalr
- jr
- mfhi
- mflo
- mthi
- mtlo
- mult
- multu
- nor
- or
- sll
- sllv
- slt
- sltu
- sra
- srav
- srl
- srlv
- sub
- subu
- syscall（部分）
- xor

支持的 I 指令：
- addi
- addiu
- andi
- beq
- bgez
- bgezal
- bgtz
- blez
- bltz
- bltzal
- bne
- lb
- lbu
- lh
- lhu
- lui
- lw
- ori
- sb
- slti
- sltiu
- sh
- sw
- xori

支持的 J 指令：
- j
- jal

支持的伪指令 (会在汇编阶段转为真指令)：
- push
- pop
- move
- shi
- inc
- dec
- not
- neg
- abs
- swap
- b
- beqz
- bnez
- beqi
- bnei
- blt
- ble
- bgt
- bge
- seq
- sne


支持的格式指令：
- .zjie
- .text
- .data
- .2byte
- .4byte
- .8byte
- .ascii
- .asciiz
- .byte
- .dword
- .half
- .space
- .word

支持的 syscall 功能：
- 1（输出整数）
- 4（输出字符串）
- 5（输入整数）
- 8（输入字符串）
- 10（退出）
- 11（输出字符）
- 12（读入字符）
- 30（读入系统时间）
- 41（随机整数）
- 42（有范围的随机整数）

### 汇编
由于是延续之前作业进行开发，MIPS_Simulator中集成了一个MIPS汇编代码的编辑和汇编窗口，可以执行汇编操作，并将汇编结果导出为.bin文件。

### 反汇编
在汇编器窗口中集成了反汇编功能，在模拟器打开.bin文件时，会自动调用这个反汇编器将机器码解析成指令和数据，显示在输出框中，同时也将相应的机器码存入到模拟器的内存中。

### 模拟运行MIPS代码
MIPS_Simulator 具有简单的模拟与调试功能。用户在点击运行后，界面上标题为“输出”的活动窗口中将会实时打印程序的输出内容，同时用户可以通过在要求输入时弹出的对话框进行数据输入，完成与程序的交互。在运行时，“内存”窗口和“寄存器”窗口的内容会随着每一步指令作出实时的变化。

可以使用 test 文件夹中的 test.bin文件来进行模拟器的运行测试

### 项目运行
- 简单地点击打包后文件夹中的可执行文件中的MIPS_Simulator.exe即可运行程序（请勿删除或移动Qt的dll动态库，否则程序将无法运行）
- 如果希望从源代码编译出项目，请确保安装了Qt 5.9及以上版本，并使用Qt Creator打开MIPS_Simulator_2.pro，选择好构建套件后进行编译即可

## 设计介绍
### 总体架构
MIPS_Simulator基本上模拟了一个简单的计算机系统，项目代码大致可以分为界面类和逻辑类，并充分使用Qt的信号/槽机制完成消息传递

界面类中，主体是两个QMainWindow类，分别对应了模拟器窗口和汇编器窗口

逻辑类中，主体是MIPS模拟器类，包含一个CPU类成员，一个Memory类成员，一个GPU类成员，其中CPU类负责指令的处理，Memory类负责内存管理，GPU类负责图形窗口的绘制。

### 设计细节
出于代码的可读性和开发效率的考虑，MIPS_Simulator有如下几个设计细节：
- 处理指令逻辑时统一将指令码转换为二进制字符串
- 使用哈希表模拟内存，内存数据排布符合标准的MIPS规范
- 在资源文件中的配置文件里定义所有指令的格式，运行初始化时读取

更多设计细节详见项目源代码

### 代码文件说明
- 汇编器窗口代码: assenblerwidger.h / assenblerwidger.cpp / assenblerwidger.ui
- 模拟器窗口代码: Mainwindow_simulator.h / Mainwindow_simulator.cpp / MIPS_simulator.ui
- 主程序: main.cpp
- 汇编器/反汇编器代码: n_assembler.h / n_assembler.cpp
  - 指令类: Instruction_Base.h /  Instruction_Base.cpp
- 模拟器代码: mips_vm.h/ mips_vm.cpp
  - CPU: vmcpu.h / vmcpu.cpp
  - 内存: vmmemory.h / vmmemory.cpp


## 心得和体会
这个项目作为计算机系统原理课程的大作业，是具有一定复杂度的，通过这个项目的开发，我对面向对象编程思想的应用有了更多认识，同时，对于计算机系统的内部原理的理解也有了进步，总体而言，这个项目给了我很多收获。