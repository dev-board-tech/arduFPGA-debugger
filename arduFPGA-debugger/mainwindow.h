#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "serialportsetup.h"
#include "flashtable.h"
#include "ramtable.h"
#include "eepromtable.h"
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
    void on_actionOpen_FLASH_File_triggered();
    void on_actionOpen_RAM_triggered();
    void on_actionOpen_EEPROM_File_triggered();
    void on_actionExit_triggered();
    void on_actionInterface_setup_triggered();
    void on_actionOpen_COM_triggered();

    void on_actionRead_FLASH_triggered();

    void on_actionRead_RAM_triggered();

    void on_actionRead_EEPROM_triggered();

private:
    Ui::MainWindow *ui;
    serialPortSetup *serialPortWindow;
    flashTable *flashTableWindow;
    ramTable *ramTableWindow;
    eepromTable *eepromTableWindow;

    QSerialPort serial;
    QString comPort;
    QString baudRate;


signals:
    void error(const QString &s);

};
#endif // MAINWINDOW_H
