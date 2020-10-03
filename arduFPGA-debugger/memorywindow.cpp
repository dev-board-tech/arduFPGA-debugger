#include "memorywindow.h"
#include "ui_memorywindow.h"
#include <QtSql/QSqlTableModel>
#include <QTableWidgetItem>

#include "def.h"

static const QStringList MEMORY_LIST = QStringList({"FLASH", "EEPROM", "RAM"});

memoryWindow::memoryWindow(QWidget *parent, QSerialPort *serial) :
    QDialog(parent),
    ui(new Ui::memoryWindow)
{
    ui->setupUi(this);

    initialized = false;


    this->serial = serial;
    scrollPositionCurrent = 0;
    lastPushedLoad = true;
    lastPushedCompare = false;
    flashMemoryArray.resize(def::flashSize);
    eepromMemoryArray.resize(def::eepromSize);
    ramMemoryArray.resize(def::ramSize);
    lastSelectedMemory = "FLASH";

    tableViewFlash = new QTableView(this);
    tableViewFlash->setGeometry(0, 0, 680, 685);
    tableViewFlash->setVisible(true);
    tableViewFlash->setStyleSheet("font: bold Consolas large");

    tableViewEeprom = new QTableView(this);
    tableViewEeprom->setGeometry(0, 0, 680, 685);
    tableViewEeprom->setVisible(false);

    tableViewRam = new QTableView(this);
    tableViewRam->setGeometry(0, 0, 680, 685);
    tableViewRam->setVisible(false);

    int rowsFlash = def::flashSize / 16;
    if(rowsFlash * 16 != def::flashSize) {
        rowsFlash++;
    }
    int rowsEeprom = def::eepromSize / 16;
    if(rowsEeprom * 16 != def::eepromSize) {
        rowsEeprom++;
    }
    int rowRam = def::ramSize / 16;
    if(rowRam * 16 != def::ramSize) {
        rowRam++;
    }

    modelFlahs = new QStandardItemModel(rowsFlash, 16, this);
    modelEeprom = new QStandardItemModel(rowsEeprom, 16, this);
    modelRam = new QStandardItemModel(rowRam, 16, this);

    tableViewFlash->setFocusPolicy(Qt::NoFocus);
    tableViewFlash->setStyleSheet("QTableView { border: none;"
                                   "background-color: lightgrey;"
                                   "selection-background-color: #999}");

    tableViewEeprom->setFocusPolicy(Qt::NoFocus);
    tableViewEeprom->setStyleSheet("QTableView { border: none;"
                                   "background-color: lightgrey;"
                                   "selection-background-color: #999}");

    tableViewRam->setFocusPolicy(Qt::NoFocus);
    tableViewRam->setStyleSheet("QTableView { border: none;"
                                   "background-color: lightgrey;"
                                   "selection-background-color: #999}");



    rfshTimer.setInterval(def::dataTableRefreshDelay);
    connect(&rfshTimer, SIGNAL(timeout()), this, SLOT(windowRfsh()));
    //rfshTimer.start();

    connect(tableViewFlash->verticalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(windowScrollEvent(int)));

    ui->memoryTypeComboBox->clear();
    ui->memoryTypeComboBox->addItems(MEMORY_LIST);

    tableViewCurrentUsed = tableViewFlash;
    if(!ui->memoryTypeComboBox->currentText().compare("FLASH")) {
        inUseMemoryArray = flashMemoryArray;
    } else if(!ui->memoryTypeComboBox->currentText().compare("EEPROM")) {
        inUseMemoryArray = eepromMemoryArray;
    } else if(!ui->memoryTypeComboBox->currentText().compare("RAM")) {
        inUseMemoryArray = ramMemoryArray;
    }
    tableViewFlash->setModel(modelFlahs);
    tableViewEeprom->setModel(modelEeprom);
    tableViewRam->setModel(modelRam);

    for (int cnt = 0;cnt < 16 ;cnt++ ) {
        tableViewFlash->setColumnWidth(cnt, 5);
        modelFlahs->setHeaderData(cnt, Qt::Horizontal, QString::asprintf("0x%01X", cnt));
    }
    for (int cnt = 0;cnt < rowsFlash ;cnt++ ) {
        tableViewFlash->setRowHeight(cnt, 5);
        modelFlahs->setHeaderData(cnt, Qt::Vertical, QString::asprintf("0x%04X", cnt * 16));
    }

    for (int cnt = 0;cnt < 16 ;cnt++ ) {
        tableViewEeprom->setColumnWidth(cnt, 5);
        modelEeprom->setHeaderData(cnt, Qt::Horizontal, QString::asprintf("0x%01X", cnt));
    }
    for (int cnt = 0;cnt < rowsFlash ;cnt++ ) {
        tableViewEeprom->setRowHeight(cnt, 5);
        modelEeprom->setHeaderData(cnt, Qt::Vertical, QString::asprintf("0x%04X", cnt * 16));
    }

    for (int cnt = 0;cnt < 16 ;cnt++ ) {
        tableViewRam->setColumnWidth(cnt, 5);
        modelRam->setHeaderData(cnt, Qt::Horizontal, QString::asprintf("0x%01X", cnt));
    }
    for (int cnt = 0;cnt < rowsFlash ;cnt++ ) {
        tableViewRam->setRowHeight(cnt, 5);
        modelRam->setHeaderData(cnt, Qt::Vertical, QString::asprintf("0x%04X", cnt * 16));
    }

    initialized = true;
}

memoryWindow::~memoryWindow()
{
    delete ui;
}

bool memoryWindow::readMemory(int readStart, int readEnd, bool load, bool compare) {
    int memSize = 0;
    if(!ui->memoryTypeComboBox->currentText().compare("FLASH")) {
        memSize = def::flashSize;
    } else if(!ui->memoryTypeComboBox->currentText().compare("EEPROM")) {
        memSize = def::eepromSize;
    } else if(!ui->memoryTypeComboBox->currentText().compare("RAM")) {
        memSize = def::ramSize;
    }

    if(readEnd > memSize - 1) {
        readEnd = memSize;
    }
    QString *data = new QString();
    QString command;
    if(!ui->memoryTypeComboBox->currentText().compare("FLASH")) {
        command = "RF";
    } else if(!ui->memoryTypeComboBox->currentText().compare("EEPROM")) {
        command = "RE";
    } else if(!ui->memoryTypeComboBox->currentText().compare("RAM")) {
        command = "RR";
    }
    if(!def::uartSendAddrRange(serial, command, readStart, readEnd)) {
        if(!def::uartReadBytes(serial, data)) {
            QByteArray array = QByteArray(def::flashSize, Qt::Initialization::Uninitialized);
            int start;
            int end;
            def::parseData(data, &array, &start, &end);

            if(end == 0 || end >= memSize) {
                end = memSize;
            }
            int startX = start % 16;
            int startY = (start - startX) / 16;
            int endX = 16;
            int endY = end / 16;
            int cntItems = start;

            for (int cntV = startY; cntV < endY; cntV++) {
                for (int cntH = startX; cntH < endX; cntH++) {
                    QStandardItem *item = nullptr;
                    if(!ui->memoryTypeComboBox->currentText().compare("FLASH")) {
                        item = modelFlahs->itemFromIndex(modelFlahs->index(cntV, cntH));
                    } else if(!ui->memoryTypeComboBox->currentText().compare("EEPROM")) {
                        item = modelEeprom->itemFromIndex(modelEeprom->index(cntV, cntH));
                    } else if(!ui->memoryTypeComboBox->currentText().compare("RAM")) {
                        item = modelRam->itemFromIndex(modelRam->index(cntV, cntH));
                    }
                    if(item != 0) {
                        char arrayValue = array.at(cntItems);
                        if(compare) {
                            if(arrayValue != inUseMemoryArray.at(cntItems) || item->text().length() == 0) {
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
                            inUseMemoryArray[cntItems] = arrayValue;
                        }
                    } else {
                        item->setForeground(QBrush(Qt::black));
                    }
                    cntItems++;
                }
            }
            if(!ui->memoryTypeComboBox->currentText().compare("FLASH")) {
                flashMemoryArray = inUseMemoryArray;
            } else if(!ui->memoryTypeComboBox->currentText().compare("EEPROM")) {
                eepromMemoryArray = inUseMemoryArray;
            } else if(!ui->memoryTypeComboBox->currentText().compare("RAM")) {
                ramMemoryArray = inUseMemoryArray;
            }
            return true;
        }
    }
    return false;
}

void memoryWindow::showEvent(QShowEvent *) {
    windowRfsh();
}

void memoryWindow::resizeEvent(QResizeEvent *event) {
    windowSize = event->size();
    tableViewCurrentUsed->setGeometry(0, 40, windowSize.rwidth(), windowSize.rheight() - 80);
    ui->refreshWindowPushButton->setGeometry(ui->refreshWindowPushButton->x(), windowSize.rheight() - 30, ui->refreshWindowPushButton->width(), ui->refreshWindowPushButton->height());
    ui->compareWindowPushButton->setGeometry(ui->compareWindowPushButton->x(), windowSize.rheight() - 30, ui->compareWindowPushButton->width(), ui->compareWindowPushButton->height());
    ui->refreshCompareWindowPushButton->setGeometry(ui->refreshCompareWindowPushButton->x(), windowSize.rheight() - 30, ui->refreshCompareWindowPushButton->width(), ui->refreshCompareWindowPushButton->height());
    ui->readMemoryPushButton->setGeometry(ui->readMemoryPushButton->x(), windowSize.rheight() - 30, ui->readMemoryPushButton->width(), ui->readMemoryPushButton->height());
    ui->writeMemoryPushButton->setGeometry(ui->writeMemoryPushButton->x(), windowSize.rheight() - 30, ui->writeMemoryPushButton->width(), ui->writeMemoryPushButton->height());
    if(ui->realtimeLoadCheckBox->isChecked()) {
        rfshTimer.start();
    }
}

void memoryWindow::windowScrollEvent(int pos) {
    if(ui->realtimeLoadCheckBox->isChecked()) {
        rfshTimer.start();
    }
    scrollPositionCurrent = pos;
}

void memoryWindow::windowRfsh() {
    rfshTimer.stop();
    int visibleItems = (tableViewCurrentUsed->geometry().height() / tableViewCurrentUsed->rowHeight(0));
    int readStart = scrollPositionCurrent * 16;
    int readEnd = ((scrollPositionCurrent + visibleItems) * 16);
    readMemory(readStart,readEnd, lastPushedLoad, lastPushedCompare);
}

void memoryWindow::on_readMemoryPushButton_clicked()
{
    if(!ui->memoryTypeComboBox->currentText().compare("FLASH")) {
        readMemory(0, def::flashSize, true, false);
    } else if(!ui->memoryTypeComboBox->currentText().compare("EEPROM")) {
        readMemory(0, def::eepromSize, true, false);
    } else if(!ui->memoryTypeComboBox->currentText().compare("RAM")) {
        readMemory(0, def::ramSize, true, false);
    }

}

void memoryWindow::on_refreshWindowPushButton_clicked()
{
    lastPushedLoad = true;
    lastPushedCompare = false;
    int visibleItems = (tableViewCurrentUsed->geometry().height() / tableViewCurrentUsed->rowHeight(0));
    int readStart = scrollPositionCurrent * 16;
    int readEnd = ((scrollPositionCurrent + visibleItems) * 16);
    readMemory(readStart,readEnd, true, false);
}

void memoryWindow::on_refreshCompareWindowPushButton_clicked()
{
    lastPushedLoad = true;
    lastPushedCompare = true;
    int visibleItems = (tableViewCurrentUsed->geometry().height() / tableViewCurrentUsed->rowHeight(0));
    int readStart = scrollPositionCurrent * 16;
    int readEnd = ((scrollPositionCurrent + visibleItems) * 16);
    readMemory(readStart,readEnd, true, true);
}

void memoryWindow::on_compareWindowPushButton_clicked()
{
    lastPushedLoad = false;
    lastPushedCompare = true;
    int visibleItems = (tableViewCurrentUsed->geometry().height() / tableViewCurrentUsed->rowHeight(0));
    int readStart = scrollPositionCurrent * 16;
    int readEnd = ((scrollPositionCurrent + visibleItems) * 16);
    readMemory(readStart,readEnd, false, true);
}

void memoryWindow::on_memoryTypeComboBox_currentIndexChanged(const QString &str)
{
    if(initialized) {
        if(!lastSelectedMemory.compare("FLASH")) {
            tableViewFlash->setVisible(false);
        } else if(!lastSelectedMemory.compare("EEPROM")) {
            tableViewEeprom->setVisible(false);
        } else if(!lastSelectedMemory.compare("RAM")) {
            tableViewRam->setVisible(false);
        }
        if(!ui->memoryTypeComboBox->currentText().compare("FLASH")) {
            inUseMemoryArray = flashMemoryArray;
            tableViewCurrentUsed = tableViewFlash;
            connect(tableViewFlash->verticalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(windowScrollEvent(int)));

        } else if(!ui->memoryTypeComboBox->currentText().compare("EEPROM")) {
            inUseMemoryArray = eepromMemoryArray;
            tableViewCurrentUsed = tableViewEeprom;
            connect(tableViewEeprom->verticalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(windowScrollEvent(int)));

        } else if(!ui->memoryTypeComboBox->currentText().compare("RAM")) {
            inUseMemoryArray = ramMemoryArray;
            tableViewCurrentUsed = tableViewRam;
            connect(tableViewRam->verticalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(windowScrollEvent(int)));

        }

        tableViewCurrentUsed->setVisible(true);


        lastSelectedMemory = str;

        tableViewCurrentUsed->setGeometry(0, 40, windowSize.rwidth(), windowSize.rheight() - 80);
        ui->refreshWindowPushButton->setGeometry(ui->refreshWindowPushButton->x(), windowSize.rheight() - 30, ui->refreshWindowPushButton->width(), ui->refreshWindowPushButton->height());
        ui->compareWindowPushButton->setGeometry(ui->compareWindowPushButton->x(), windowSize.rheight() - 30, ui->compareWindowPushButton->width(), ui->compareWindowPushButton->height());
        ui->refreshCompareWindowPushButton->setGeometry(ui->refreshCompareWindowPushButton->x(), windowSize.rheight() - 30, ui->refreshCompareWindowPushButton->width(), ui->refreshCompareWindowPushButton->height());
        ui->readMemoryPushButton->setGeometry(ui->readMemoryPushButton->x(), windowSize.rheight() - 30, ui->readMemoryPushButton->width(), ui->readMemoryPushButton->height());
        ui->writeMemoryPushButton->setGeometry(ui->writeMemoryPushButton->x(), windowSize.rheight() - 30, ui->writeMemoryPushButton->width(), ui->writeMemoryPushButton->height());
    }
}

