#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "def.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(mainTitle);
    serialPortWindow = new serialPortSetup(this);
    memoryWin = new memoryWindow(this, &serial);
    ioWin = new ioWindow(this, &serial);
}

MainWindow::~MainWindow()
{
    delete serialPortWindow;
    delete memoryWin;
    delete ioWin;
    delete ui;
}


void MainWindow::on_actionInterface_setup_triggered()
{
    serialPortWindow->exec();
    if(serialPortWindow->Accepted == QDialog::DialogCode::Accepted) {
        if(comPort != serialPortWindow->comPort || baudRate != serialPortWindow->baudRate) {
            comPort = serialPortWindow->comPort;
            baudRate = serialPortWindow->baudRate;
            if(!QString::compare(ui->actionOpen_COM->text(),"Close COM")) {
                serial.close();
                serial.setPortName(comPort);
                serial.setBaudRate(baudRate.toInt());
                if (!serial.open(QIODevice::ReadWrite)) {
                    emit error(tr("Can't open %1, error code %2")
                               .arg(comPort).arg(serial.error()));
                    ui->actionOpen_COM->setText("Open COM");
                    this->setWindowTitle(mainTitle);
                    return;
                } else {
                    ui->actionOpen_COM->setText("Close COM");
                    this->setWindowTitle(mainTitle + " | " + comPort + " " + baudRate);
                }
            }
        }
    } else if(serialPortWindow->Accepted == QDialog::DialogCode::Rejected) {
    }
}

void MainWindow::on_actionOpen_COM_triggered()
{
    if(!QString::compare(ui->actionOpen_COM->text(),"Open COM")) {
        serial.close();
        serial.setPortName(comPort);
        serial.setBaudRate(baudRate.toInt());
        if (!serial.open(QIODevice::ReadWrite)) {
            emit error(tr("Can't open %1, error code %2")
                       .arg(comPort).arg(serial.error()));
            ui->actionOpen_COM->setText("Open COM");
            this->setWindowTitle(mainTitle);
            return;
        } else {
            ui->actionOpen_COM->setText("Close COM");
            this->setWindowTitle(mainTitle + " | " + comPort + " " + baudRate);
        }
    } else {
        serial.close();
        ui->actionOpen_COM->setText("Open COM");
        this->setWindowTitle(mainTitle);
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}



void MainWindow::on_actionMemory_triggered()
{
    memoryWin->show();
}

void MainWindow::on_actionIO_triggered()
{
    ioWin->show();
}
