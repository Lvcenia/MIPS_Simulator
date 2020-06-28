#ifndef VMGPU_H
#define VMGPU_H
#define VRAM_NUM 0x7fff
#include <QObject>
#include "displaywidget.h"


class vmGPU : public QObject
{
    Q_OBJECT
public:
    explicit vmGPU(QObject *parent = nullptr);


signals:
    void SetDispalyPixelColor(int x, int y, QColor color);

private:
    DisplayWidget* displayWidget;
    uint16_t VRAM[VRAM_NUM];

};

#endif // VMGPU_H
