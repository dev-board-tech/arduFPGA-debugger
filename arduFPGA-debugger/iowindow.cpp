#include "iowindow.h"
#include "ui_iowindow.h"

ioWindow::ioWindow(QWidget *parent, QSerialPort *serial) :
    QDialog(parent),
    ui(new Ui::ioWindow)
{
    ui->setupUi(this);

    this->serial = serial;
}

ioWindow::~ioWindow()
{
    delete ui;
}
