#include "finclip-qt-demo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FinclipQtDemo w;
    w.show();
    return a.exec();
}
