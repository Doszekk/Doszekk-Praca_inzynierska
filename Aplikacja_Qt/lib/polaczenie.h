#ifndef POLACZENIE_H
#define POLACZENIE_H

#include <QDialog>
#include <QSerialPort>



namespace Ui {
class Polaczenie;
}

class Polaczenie : public QDialog
{
    Q_OBJECT

public:
    explicit Polaczenie(QWidget *parent = nullptr);
    ~Polaczenie();
    QSerialPort *Arduino;


private slots:
    void on_pushButton_Polacz_clicked();
    void on_pushButton_Szukaj_clicked();
    void Napisz_w_logu(QString wiadomosc);
    void on_pushButton_Rozlacz_clicked();

private:
    Ui::Polaczenie *ui;
    QList<QSerialPortInfo> dostepnePorty;


};

#endif // POLACZENIE_H
