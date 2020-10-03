#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "serialportsetup.h"
#include "memorywindow.h"
#include "iowindow.h"
#include "qserialport.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionInterface_setup_triggered();

    void on_actionOpen_COM_triggered();
    void on_actionExit_triggered();

    void on_actionMemory_triggered();
    void on_actionIO_triggered();

private:
    Ui::MainWindow *ui;
    serialPortSetup *serialPortWindow;
    memoryWindow *memoryWin;
    ioWindow *ioWin;

    QSerialPort serial;
    QString comPort;
    QString baudRate;


signals:
    void error(const QString &s);

};
#endif // MAINWINDOW_H
