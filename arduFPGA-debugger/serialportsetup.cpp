#include "serialportsetup.h"
#include "ui_serialportsetup.h"

#include <QtSerialPort/QSerialPortInfo>

serialPortSetup::serialPortSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serialPortSetup)
{
    ui->setupUi(this);

    insideEvent = false;

    const auto infos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &info : infos)
        ui->comPortComboBox->addItem(info.portName());
    if(infos.count() > 0) {
       ui->comPortComboBox->setCurrentIndex(0);
    }

    if(infos.count() > 0) {
        QList<qint32> baudRatesList = QSerialPortInfo::standardBaudRates();
        for(const qint32 &baudRate : baudRatesList)
            ui->baudRateComboBox->addItem(QString::number(baudRate));
        ui->baudRateComboBox->setCurrentText("115200");
    }
    baudRate = "115200";
}

serialPortSetup::~serialPortSetup()
{
    delete ui;
}

void serialPortSetup::showEvent(QShowEvent *) {
    insideEvent = true;
    const auto infos = QSerialPortInfo::availablePorts();
    ui->comPortComboBox->clear();
    for (const QSerialPortInfo &info : infos)
        ui->comPortComboBox->addItem(info.portName());
    if(comPort.length() > 0) {
    ui->comPortComboBox->setCurrentText(comPort);
    } else if(infos.count() > 0) {
       ui->comPortComboBox->setCurrentIndex(0);
    }

    ui->baudRateComboBox->clear();
    if(infos.count() > 0) {
        QList<qint32> baudRatesList = QSerialPortInfo::standardBaudRates();
        for(const qint32 &baudRate : baudRatesList)
            ui->baudRateComboBox->addItem(QString::number(baudRate));
        ui->baudRateComboBox->setCurrentText(baudRate);
    }
    insideEvent = false;
}

void serialPortSetup::on_comPortComboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1.length() > 0 && insideEvent == false)
        comPort = arg1;
}

void serialPortSetup::on_baudRateComboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1.length() > 0 && insideEvent == false)
        baudRate = arg1;
}
