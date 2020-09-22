#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtXml>
#include <QtWidgets>
#include <QtCore>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void open();
    void openFB2(QFile *f);
    void openTXT(QFile *f);
    void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;

    QStringList /*tokens,*/ thisToken;
};

#endif // MAINWINDOW_H
