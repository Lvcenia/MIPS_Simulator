#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QDockWidget>

namespace Ui {
class DisplayWidget;
}

class DisplayWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit DisplayWidget(QWidget *parent = nullptr);
    ~DisplayWidget();

private:
    Ui::DisplayWidget *ui;
};

#endif // DISPLAYWIDGET_H
