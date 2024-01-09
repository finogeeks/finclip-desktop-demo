#ifndef FINCLIPQTDEMO_H
#define FINCLIPQTDEMO_H

#include <QMainWindow>
#include "finclip_api.h"

class FinclipQtDemo : public QMainWindow
{
    Q_OBJECT

public:
    FinclipQtDemo(QWidget *parent = nullptr);
    ~FinclipQtDemo();
private:
    void resizeEvent(QResizeEvent* event);
};
#endif // FINCLIPQTDEMO_H
