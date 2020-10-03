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
    memoryWin[0] = new memoryWindow(this, &serial);
    memoryWin[1] = new memoryWindow(this, &serial);
    memoryWin[2] = new memoryWindow(this, &serial);
    memoryWin[3] = new memoryWindow(this, &serial);
    memoryWin[4] = new memoryWindow(this, &serial);
    memoryWin[5] = new memoryWindow(this, &serial);
    ioWin[0] = new ioWindow(this, &serial);
    ioWin[1] = new ioWindow(this, &serial);
    ioWin[2] = new ioWindow(this, &serial);
    ioWin[3] = new ioWindow(this, &serial);
    ioWin[4] = new ioWindow(this, &serial);
    ioWin[5] = new ioWindow(this, &serial);
    ioWin[6] = new ioWindow(this, &serial);
    ioWin[7] = new ioWindow(this, &serial);
    ioWin[8] = new ioWindow(this, &serial);
    ioWin[9] = new ioWindow(this, &serial);
    ioWin[10] = new ioWindow(this, &serial);
    ioWin[11] = new ioWindow(this, &serial);
    ioWin[12] = new ioWindow(this, &serial);
    ioWin[13] = new ioWindow(this, &serial);
    ioWin[14] = new ioWindow(this, &serial);
    ioWin[15] = new ioWindow(this, &serial);

    fileDialog = new QFileDialog(parent);
    fileDialog->setReadOnly(true);

}

MainWindow::~MainWindow()
{
    delete serialPortWindow;
    delete memoryWin[5];
    delete memoryWin[4];
    delete memoryWin[3];
    delete memoryWin[2];
    delete memoryWin[1];
    delete memoryWin[0];
    delete ioWin[15];
    delete ioWin[14];
    delete ioWin[13];
    delete ioWin[12];
    delete ioWin[11];
    delete ioWin[10];
    delete ioWin[9];
    delete ioWin[8];
    delete ioWin[7];
    delete ioWin[6];
    delete ioWin[5];
    delete ioWin[4];
    delete ioWin[3];
    delete ioWin[2];
    delete ioWin[1];
    delete ioWin[0];
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



void MainWindow::on_actionOpen_File_triggered()
{
    fileDialog->getOpenFileName(this, "Open ROM file", QString() ,QString("*.hex *.bin"));
}

void MainWindow::on_actionOpen_Memory_Window_1_triggered()
{
    memoryWin[0]->show();
}

void MainWindow::on_actionOpen_Memory_Window_2_triggered()
{
    memoryWin[1]->show();
}

void MainWindow::on_actionOpen_Memory_Window_3_triggered()
{
    memoryWin[2]->show();
}

void MainWindow::on_actionOpen_Memory_Window_4_triggered()
{
    memoryWin[3]->show();
}

void MainWindow::on_actionOpen_Memory_Window_5_triggered()
{
    memoryWin[4]->show();
}

void MainWindow::on_actionOpen_Memory_Window_6_triggered()
{
    memoryWin[5]->show();
}


void MainWindow::on_actionOpen_IO_Window_1_triggered()
{
    ioWin[0]->show();
}

void MainWindow::on_actionOpen_IO_Window_2_triggered()
{
    ioWin[1]->show();
}

void MainWindow::on_actionOpen_IO_Window_3_triggered()
{
    ioWin[2]->show();
}

void MainWindow::on_actionOpen_IO_Window_4_triggered()
{
    ioWin[3]->show();
}

void MainWindow::on_actionOpen_IO_Window_5_triggered()
{
    ioWin[4]->show();
}

void MainWindow::on_actionOpen_IO_Window_6_triggered()
{
    ioWin[5]->show();
}

void MainWindow::on_actionOpen_IO_Window_7_triggered()
{
    ioWin[6]->show();
}

void MainWindow::on_actionOpen_IO_Window_8_triggered()
{
    ioWin[7]->show();
}

void MainWindow::on_actionOpen_IO_Window_9_triggered()
{
    ioWin[8]->show();
}

void MainWindow::on_actionOpen_IO_Window_10_triggered()
{
    ioWin[9]->show();
}

void MainWindow::on_actionOpen_IO_Window_11_triggered()
{
    ioWin[10]->show();
}

void MainWindow::on_actionOpen_IO_Window_12_triggered()
{
    ioWin[11]->show();
}

void MainWindow::on_actionOpen_IO_Window_13_triggered()
{
    ioWin[12]->show();
}

void MainWindow::on_actionOpen_IO_Window_14_triggered()
{
    ioWin[13]->show();
}

void MainWindow::on_actionOpen_IO_Window_15_triggered()
{
    ioWin[14]->show();
}

void MainWindow::on_actionOpen_IO_Window_16_triggered()
{
    ioWin[15]->show();
}
