#include "mainwindow.h"
#include <QApplication>
/*
 * простая читалка fb2-файлов
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
