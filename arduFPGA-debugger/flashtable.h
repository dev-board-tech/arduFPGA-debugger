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
class flashTable;
}

class flashTable : public QDialog
{
    Q_OBJECT

public:
    explicit flashTable(QWidget *parent = nullptr, QSerialPort *serial = nullptr);
    ~flashTable();
    bool readMemory(int, int);

private:
    Ui::flashTable *ui;
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

#endif // FLASHTABLE_H