#include "ramtable.h"
#include "QScrollBar"
#include "ui_ramtable.h"

#include "def.h"

ramTable::ramTable(QWidget *parent, QSerialPort *serial) :
    QDialog(parent),
    ui(new Ui::ramTable)
{
    ui->setupUi(this);
    this->serial = serial;
    scrollPosition = 0;
    lastPushedLoad = true;
    lastPushedCompare = false;
    memoryArray.resize(def::ramSize);

    int rows = def::ramSize / 16;
    if(rows * 16 != def::ramSize) {
        rows++;
    }
    tableView = new QTableView(this);
    tableView->installEventFilter(this);
    tableView->setGeometry(0, 0, 680, 685);
    model = new QStandardItemModel(rows, 16, this);
    tableView->setModel(model);
    tableView->setWindowTitle("HH");
    tableView->setAutoFillBackground(true);
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

ramTable::~ramTable()
{
    delete ui;
}

bool ramTable::readMemory(int readStart, int readEnd, bool load, bool compare) {
    if(readEnd > def::ramSize - 1) {
        readEnd = def::ramSize;
    }
    QString *data = new QString();
    if(!def::uartSendAddrRange(serial, "RR", readStart, readEnd)) {
        if(!def::uartReadBytes(serial, data)) {
            QByteArray array = QByteArray(def::ramSize, Qt::Initialization::Uninitialized);
            int start;
            int end;
            def::parseData(data, &array, &start, &end);

            if(end == 0 || end >= def::ramSize) {
                end = def::ramSize;
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
                            if(arrayValue != memoryArray.at(cntItems) || item->text().length() == 0) {
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

void ramTable::showEvent(QShowEvent *) {
    /*if(serial->isOpen() == false) {
        for (int cntV = 0; cntV < model->columnCount(); cntV++) {
            for (int cntH = 0; cntH < model->rowCount(); cntH++) {
                QStandardItem *item = model->itemFromIndex(model->index(cntH, cntV));
                if(item->text().length() == 0)
                    item->setText("0xXX");
            }
        }
    }*/
}

void ramTable::resizeEvent(QResizeEvent *event) {
    QSize size = event->size();
    tableView->setGeometry(0, 40, size.rwidth(), size.rheight() - 40);
    rfshTimer.start();
}

void ramTable::windowScrollEvent(int pos) {
    if(ui->realtimeLoadCheckBox->isChecked()) {
        rfshTimer.start();
    }
    scrollPosition = pos;
}

void ramTable::windowRfsh() {
    rfshTimer.stop();
    int visibleItems = (tableView->geometry().height() / tableView->rowHeight(0));
    int readStart = scrollPosition * 16;
    int readEnd = ((scrollPosition + visibleItems) * 16);
    readMemory(readStart,readEnd, lastPushedLoad, lastPushedCompare);
}

void ramTable::on_readMemoryPushButton_clicked()
{
    readMemory(0, def::ramSize, true, false);
}

void ramTable::on_refreshWindowPushButton_clicked()
{
    lastPushedLoad = true;
    lastPushedCompare = false;
    int visibleItems = (tableView->geometry().height() / tableView->rowHeight(0));
    int readStart = scrollPosition * 16;
    int readEnd = ((scrollPosition + visibleItems) * 16);
    readMemory(readStart,readEnd, true, false);
}

void ramTable::on_refreshCompareWindowPushButton_clicked()
{
    lastPushedLoad = true;
    lastPushedCompare = true;
    int visibleItems = (tableView->geometry().height() / tableView->rowHeight(0));
    int readStart = scrollPosition * 16;
    int readEnd = ((scrollPosition + visibleItems) * 16);
    readMemory(readStart,readEnd, true, true);
}

void ramTable::on_compareWindowPushButton_clicked()
{
    lastPushedLoad = false;
    lastPushedCompare = true;
    int visibleItems = (tableView->geometry().height() / tableView->rowHeight(0));
    int readStart = scrollPosition * 16;
    int readEnd = ((scrollPosition + visibleItems) * 16);
    readMemory(readStart,readEnd, false, true);
}
