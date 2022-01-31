#include "polaczenie.h"
#include "ui_polaczenie.h"
#include <QList>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QSerialPort>



Polaczenie::Polaczenie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Polaczenie)
{
    ui->setupUi(this);
    this->Arduino = new QSerialPort();

    on_pushButton_Szukaj_clicked();
}

Polaczenie::~Polaczenie()
{
    delete ui;
}


void Polaczenie::on_pushButton_Polacz_clicked()
{
    if(ui->comboBox->count() == 0) {
      this->Napisz_w_logu("Brak urządzeń");
      return;
    }
    QString comboBoxQString = ui->comboBox->currentText();
    QStringList portList = comboBoxQString.split("\t");
    QString portName = portList.first();
    Arduino->setPortName(portName);

    // OTWÓRZ I SKONFIGURUJ PORT:
    if(!Arduino->isOpen())
    {
        Arduino->open(QSerialPort::ReadWrite);

        if (Arduino->isOpen())
        {
            Arduino->setBaudRate(QSerialPort::Baud19200);
            Arduino->setDataBits(QSerialPort::Data8);
            Arduino->setParity(QSerialPort::NoParity);
            Arduino->setStopBits(QSerialPort::OneStop);
            Arduino->setFlowControl(QSerialPort::NoFlowControl);
            Napisz_w_logu("Otwarcie portu szeregowego powiodło się!");
            //connect(this->Arduino, SIGNAL(readyRead()),
            //        this, SLOT(OdczytajZPortu()));
        }
        else
        {
          Napisz_w_logu("Otwarcie portu szeregowego nie powiodło się!");
        }
    }
}


void Polaczenie::on_pushButton_Szukaj_clicked()
{
    Napisz_w_logu("Szukanie...");
    ui->comboBox->clear();
    dostepnePorty.clear();
    dostepnePorty.append(QSerialPortInfo::availablePorts());
    for(int i = 0; i < dostepnePorty.count(); i++)
    {
        if(0==i)
        {
            Napisz_w_logu("Znaleźione urządzenia:");
        }
        ui->comboBox->addItem(dostepnePorty.at(i).portName() + "\t" +
                                dostepnePorty.at(i).description());
        Napisz_w_logu(dostepnePorty.at(i).portName() + "\t" +
                      dostepnePorty.at(i).description());
        if(0==dostepnePorty.count())
        {
           Napisz_w_logu("Brak");
        }
    }
}


void Polaczenie::Napisz_w_logu(QString wiadomosc)
{
    QString aktualny_czas = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->Log->append(aktualny_czas + "\t" + wiadomosc);
}


void Polaczenie::on_pushButton_Rozlacz_clicked()
{
    if(Arduino->isOpen())
    {
      Arduino->close();
      Napisz_w_logu("Zamknięto połączenie.");
    }
    else
    {
      Napisz_w_logu("Port nie jest otwarty!");
    }
}

