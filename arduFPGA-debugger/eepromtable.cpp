#include "eepromtable.h"
#include "ui_eepromtable.h"

#include "def.h"

eepromTable::eepromTable(QWidget *parent, QSerialPort *serial) :
    QDialog(parent),
    ui(new Ui::eepromTable)
{
    ui->setupUi(this);
    this->serial = serial;
    scrollPosition = 0;
    lastPushedLoad = false;
    lastPushedCompare = false;
    memoryArray.resize(def::eepromSize);


    int rows = def::eepromSize / 16;
    if(rows * 16 != def::eepromSize) {
        rows++;
    }

    tableView = new QTableView(this);
    tableView->setGeometry(0, 0, 680, 685);
    model = new QStandardItemModel(rows, 16, this);
    tableView->setModel(model);
    tableView->setWindowTitle("HH");
    //model->setRowCount(4096);

    tableView->setFocusPolicy(Qt::NoFocus);
    tableView->setStyleSheet("QTableView { border: none;"
                                   "background-color: lightgrey;"
                                   "selection-background-color: #999}");

    for (int cnt = 0;cnt < 16 ;cnt++ ) {
        tableView->setColumnWidth(cnt, 5);
        model->setHeaderData(cnt, Qt::Horizontal, QString::asprintf("0x%01X", cnt));
    }
    for (int cnt = 0;cnt < rows ;cnt++ ) {
        tableView->setRowHeight(cnt, 5);
        //QString tmp = QString::number(cnt, 16);
        model->setHeaderData(cnt, Qt::Vertical, QString::asprintf("0x%04X", cnt * 16));
    }

    //listViewDelegate delegate;
    //view->setItemDelegate(&delegate);
    tableView->show();

    rfshTimer.setInterval(def::dataTableRefreshDelay);
    connect(&rfshTimer, SIGNAL(timeout()), this, SLOT(windowRfsh()));
    rfshTimer.start();

    connect(tableView->verticalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(windowScrollEvent(int)));
}

eepromTable::~eepromTable()
{
    delete ui;
}

bool eepromTable::readMemory(int readStart, int readEnd, bool load, bool compare) {
    if(readEnd > def::eepromSize - 1) {
        readEnd = def::eepromSize;
    }
    QString *data = new QString();
    if(!def::uartSendAddrRange(serial, "RE", readStart, readEnd)) {
        if(!def::uartReadBytes(serial, data)) {
            QByteArray array = QByteArray(def::eepromSize, Qt::Initialization::Uninitialized);
            int start;
            int end;
            def::parseData(data, &array, &start, &end);

            if(end == 0 || end >= def::eepromSize) {
                end = def::eepromSize;
            }
            int startX = start % 16;
            int startY = (start - startX) / 16;
            int endX = 16;
            int endY = end / 16;
            int cntItems = start;

            for (int cntV = startY; cntV < endY; cntV++) {
                for (int cntH = startX; cntH < endX; cntH++) {
                    QStandardItem *item = model->itemFromIndex(model->index(cntV, cntH));
                    if(item != 0) {
                        char arrayValue = array.at(cntItems);
                        if(compare) {
                            if(arrayValue != memoryArray.at(cntItems)) {
                                item->setText(QString::asprintf("%02X", (uint8_t)arrayValue));
                                item->setForeground(QBrush(Qt::red));
                            } else {
                                item->setText(QString::asprintf("%02X", (uint8_t)arrayValue));
                                if(load) {
                                    item->setForeground(QBrush(Qt::black));
                                }
                            }
                        }
                        if(load) {
                            if(!compare) {
                                item->setText(QString::asprintf("%02X", (uint8_t)arrayValue));
                                item->setForeground(QBrush(Qt::black));
                            }
                            memoryArray[cntItems] = arrayValue;
                        }
                    } else {
                        item->setForeground(QBrush(Qt::black));
                    }
                    cntItems++;
                }
            }
            return true;
        }
    }
    return false;
}

void eepromTable::showEvent(QShowEvent *) {
/*    if(!serial->isOpen()){
        for (int cntV = 0; cntV < model->columnCount(); cntV++) {
            for (int cntH = 0; cntH < model->rowCount(); cntH++) {
                QStandardItem *item = model->itemFromIndex(model->index(cntH, cntV));
                if(item->text().length() == 0)
                    item->setText("0xXX");
            }
        }
    }*/
}

void eepromTable::resizeEvent(QResizeEvent *event) {
    QSize size = event->size();
    tableView->setGeometry(0, 40, size.rwidth(), size.rheight() - 40);
    rfshTimer.start();
}

void eepromTable::windowScrollEvent(int pos) {
    if(ui->realtimeLoadCheckBox->isChecked()) {
        rfshTimer.start();
    }
    scrollPosition = pos;
}

void eepromTable::windowRfsh() {
    rfshTimer.stop();
    int visibleItems = (tableView->geometry().height() / tableView->rowHeight(0));
    int readStart = scrollPosition * 16;
    int readEnd = ((scrollPosition + visibleItems) * 16);
    readMemory(readStart,readEnd, lastPushedLoad, lastPushedCompare);
}

void eepromTable::on_readMemoryPushButton_clicked()
{
    readMemory(0, def::eepromSize, true, false);
}

void eepromTable::on_refreshWindowPushButton_clicked()
{
    lastPushedLoad = true;
    lastPushedCompare = false;
    int visibleItems = (tableView->geometry().height() / tableView->rowHeight(0));
    int readStart = scrollPosition * 16;
    int readEnd = ((scrollPosition + visibleItems) * 16);
    readMemory(readStart,readEnd, true, false);
}

void eepromTable::on_refreshCompareWindowPushButton_clicked()
{
    lastPushedLoad = true;
    lastPushedCompare = true;
    int visibleItems = (tableView->geometry().height() / tableView->rowHeight(0));
    int readStart = scrollPosition * 16;
    int readEnd = ((scrollPosition + visibleItems) * 16);
    readMemory(readStart,readEnd, true, true);
}

void eepromTable::on_compareWindowPushButton_clicked()
{
    lastPushedLoad = false;
    lastPushedCompare = true;
    int visibleItems = (tableView->geometry().height() / tableView->rowHeight(0));
    int readStart = scrollPosition * 16;
    int readEnd = ((scrollPosition + visibleItems) * 16);
    readMemory(readStart,readEnd, false, true);
}
