#ifndef SERIALPORTSETUP_H
#define SERIALPORTSETUP_H

#include <QDialog>

namespace Ui {
class serialPortSetup;
}

class serialPortSetup : public QDialog
{
    Q_OBJECT

public:
    explicit serialPortSetup(QWidget *parent = nullptr);
    ~serialPortSetup();

    QString comPort;
    QString baudRate;

private slots:
    void on_comPortComboBox_currentIndexChanged(const QString &arg1);

    void on_baudRateComboBox_currentIndexChanged(const QString &arg1);

protected:
    void showEvent(QShowEvent *) override;

private:
    Ui::serialPortSetup *ui;
    bool insideEvent;
};

#endif // SERIALPORTSETUP_H
