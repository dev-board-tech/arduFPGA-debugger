#ifndef EEPROMTABLE_H
#define EEPROMTABLE_H

#include <QDialog>
#include "QStandardItemModel"
#include <QTableView>
#include <QResizeEvent>
#include <QTimer>
#include "QScrollBar"
#include <QSerialPort>

namespace Ui {
class eepromTable;
}

class eepromTable : public QDialog
{
    Q_OBJECT

public:
    explicit eepromTable(QWidget *parent = nullptr, QSerialPort *serial = nullptr);
    ~eepromTable();
    bool readMemory(int, int);

private:
    Ui::eepromTable *ui;
    QTableView *tableView;
    QStandardItemModel *model;
    QSerialPort *serial;
    QTimer rfshTimer;
    int scrollPosition;

protected:
    void showEvent(QShowEvent *) override;
    virtual void resizeEvent(QResizeEvent *) override;
public slots:
    void windowScrollEvent(int);
    void windowRfsh();
private slots:
    void on_readMemoryPushButton_clicked();
};

#endif // EEPROMTABLE_H
