#ifndef DEF_H
#define DEF_H

#include <QString>
#include <QSerialPort>
#include <QStringList>

static const QString mainTitle = QString("arduFPGA debugger V0.1 (morgoth@devboard.tech)");

class def {
public:

static const int dataTableRefreshDelay = 500;

static const int flashSize = 0x10000;
static const int ramSize = 0x8000;
static const int eepromSize = 0x400;

static bool uartSendReceiveChar(QSerialPort *serial, char c) {
    bool err = false;
    QByteArray arr;
    char cRx;
    arr.append(c);
    if(serial->write(arr) == -1) {
        err = true;
    } else {
        if(serial->waitForReadyRead(5)) {
            QByteArray responseData = serial->readAll();
            if(responseData.count() == 1) {
                cRx = responseData[0];
                if(c != cRx) {
                    err = true;
                } else {
                    err = false;
                }
            } else {
                err = true;
            }
        } else {
            err = true;
        }
    }
    return err;
}

static bool uartReadBytes(QSerialPort *serial, QString *data) {
    bool err = false;
    if(serial->isOpen()) {
        uartSendReceiveChar(serial, '\r');
        while(1) {
            bool newData = false;
            if(serial->waitForReadyRead(50)) {
                QByteArray responseData = serial->readAll();
                data->append(responseData);
                newData = true;
            } else {
                if(data->length() == 0) {
                    err = true;
                }
            }
            if(!newData) {
                break;
            }
        }
    }
    //serial->clear();
    //serial->clearError();
    return err;
}

static bool uartSendAddrRange(QSerialPort *serial, QString cmd, int start, int end) {
    bool err = false;
    std::string str = cmd.mid(0, 2).toStdString();
    QString command = QString::asprintf("%s:%04X-%04X", str.c_str(), start % 0x10000, end % 0x10000);
    if(serial->isOpen()) {
        for(char ch : command.toStdString()) {
            if(def::uartSendReceiveChar(serial, ch)) {
                err = true;
                break;
            }
        }
    } else {
        err = true;
    }
    return err;
}

static bool parseData(QString *data, QByteArray *array, int *start, int *end) {
    QStringList list = data->split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    bool startAddrExtracted = false;
    int startCnt = 0;
    for(QString str : list) {
        QStringList line = str.remove('>').remove(' ').split(QRegExp("[:]"));
        if(line.count() != 2) {
            return false;
        }
        bool bStatus = false;
        int beginConvert = line[0].toInt(&bStatus, 16);
        int endConvert = (line[1].length() / 2);
        if(!startAddrExtracted) {
            *start = beginConvert;
            startCnt = beginConvert;
            startAddrExtracted = true;
        }
        for(int cnt = 0; cnt < endConvert; cnt++) {
            char a = line[1].mid(cnt * 2, 2).toInt(&bStatus, 16);
            if(beginConvert + cnt >= array->count())
                break;
            array->data()[beginConvert + cnt] = a;
            startCnt++;
        }
    }
    *end = startCnt;
    return true;
}


};

#endif // DEF_H
