#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

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

    void on_actionOpen_File_triggered();

    void on_actionOpen_Memory_Window_1_triggered();
    void on_actionOpen_Memory_Window_2_triggered();
    void on_actionOpen_Memory_Window_3_triggered();
    void on_actionOpen_Memory_Window_4_triggered();
    void on_actionOpen_Memory_Window_5_triggered();
    void on_actionOpen_Memory_Window_6_triggered();

    void on_actionOpen_IO_Window_1_triggered();
    void on_actionOpen_IO_Window_2_triggered();
    void on_actionOpen_IO_Window_3_triggered();
    void on_actionOpen_IO_Window_4_triggered();
    void on_actionOpen_IO_Window_5_triggered();
    void on_actionOpen_IO_Window_6_triggered();
    void on_actionOpen_IO_Window_7_triggered();
    void on_actionOpen_IO_Window_8_triggered();
    void on_actionOpen_IO_Window_9_triggered();
    void on_actionOpen_IO_Window_10_triggered();
    void on_actionOpen_IO_Window_11_triggered();
    void on_actionOpen_IO_Window_12_triggered();
    void on_actionOpen_IO_Window_13_triggered();
    void on_actionOpen_IO_Window_14_triggered();
    void on_actionOpen_IO_Window_15_triggered();
    void on_actionOpen_IO_Window_16_triggered();

private:
    Ui::MainWindow *ui;
    serialPortSetup *serialPortWindow;
    memoryWindow *memoryWin[6];
    ioWindow *ioWin[16];

    QSerialPort serial;
    QString comPort;
    QString baudRate;

    QFileDialog *fileDialog;

signals:
    void error(const QString &s);

};
#endif // MAINWINDOW_H
