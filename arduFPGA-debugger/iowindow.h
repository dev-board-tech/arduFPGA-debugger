#ifndef IOWINDOW_H
#define IOWINDOW_H

#include <QDialog>
#include <QSerialPort>

namespace Ui {
class ioWindow;
}

class ioWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ioWindow(QWidget *parent = nullptr, QSerialPort *serial = nullptr);
    ~ioWindow();

private:
    Ui::ioWindow *ui;
    QSerialPort *serial;
};

#endif // IOWINDOW_H
