#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if 0
    MainWindow w;
    w.show();
    return a.exec();
#else
    MainWindow* mainwindow = MainWindow::getInstance();
    mainwindow->show();
    return a.exec();
#endif
}
