#ifndef RAMTABLE_H
#define RAMTABLE_H

#include <QDialog>
#include "QStandardItemModel"
#include <QTableView>
#include <QResizeEvent>
#include <QTimer>
#include <QSerialPort>

namespace Ui {
class ramTable;
}

class ramTable : public QDialog
{
    Q_OBJECT

public:
    explicit ramTable(QWidget *parent = nullptr, QSerialPort *serial = nullptr);
    ~ramTable();
    bool readMemory(int, int, bool, bool);

private:
    Ui::ramTable *ui;
    QTableView *tableView;
    QStandardItemModel *model;
    QSerialPort *serial;
    QTimer rfshTimer;
    int scrollPosition;
    QByteArray memoryArray;
    bool lastPushedLoad;
    bool lastPushedCompare;

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
};

#endif // RAMTABLE_H
