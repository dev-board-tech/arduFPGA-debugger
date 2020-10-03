#ifndef FLASHTABLE_H
#define FLASHTABLE_H

#include <QDialog>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include "QStandardItemModel"
#include <QTableView>
#include <QResizeEvent>
#include <QTimer>
#include "QScrollBar"
#include <QSerialPort>


namespace Ui {
class memoryWindow;
}

class memoryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit memoryWindow(QWidget *parent = nullptr, QSerialPort *serial = nullptr);
    ~memoryWindow();
    bool readMemory(int, int, bool, bool);

private:
    Ui::memoryWindow *ui;
    QTableView *tableViewFlash;
    QTableView *tableViewEeprom;
    QTableView *tableViewRam;
    QTableView *tableViewCurrentUsed;
    QStandardItemModel *modelFlahs;
    QStandardItemModel *modelEeprom;
    QStandardItemModel *modelRam;
    QSerialPort *serial;
    QTimer rfshTimer;
    int scrollPositionCurrent;
    QString lastSelectedMemory;
    QByteArray flashMemoryArray;
    QByteArray eepromMemoryArray;
    QByteArray ramMemoryArray;
    QByteArray inUseMemoryArray;
    bool lastPushedLoad;
    bool lastPushedCompare;
    bool initialized;
    QSize windowSize;

protected:
    void showEvent(QShowEvent *) override;
    virtual void resizeEvent(QResizeEvent *) override;
public slots:
    void windowScrollEvent(int);
    void windowRfsh();
private slots:
    void on_readMemoryPushButton_clicked();
    void on_refreshWindowPushButton_clicked();
    void on_refreshCompareWindowPushButton_clicked();
    void on_compareWindowPushButton_clicked();
    void on_memoryTypeComboBox_currentIndexChanged(const QString &arg1);
};

#endif // FLASHTABLE_H
