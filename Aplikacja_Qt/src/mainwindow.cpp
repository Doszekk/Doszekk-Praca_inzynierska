#include <QDebug>
#include <QList>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QSerialPort>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileDialog>
#include <QElapsedTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);





    timer.start();

    //tworzenie licznika
    timerresetUI = new QTimer(this);
    connect(timerresetUI, SIGNAL(timeout()),
            this, SLOT(resetUI()));

    //tworzenie slotów z action barów
    connect(ui->actionPolacz,SIGNAL(triggered()),
            this,SLOT(ActionPolaczTriggered()));

    //tworzenie slotów do komunikacji
    connect(this->okno_polaczenie.Arduino, SIGNAL(readyRead()),
            this, SLOT(OdczytajZPortu()));

    CalyPlik = "Czas,Temperatura 1,Temperatura 2,Temperatura 3,Cisnienie,Moment\n";

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::WyslanieRamkiDanych()
{
    QString Wiadomosc;
    QString TempMaxTemp_1 = ui->MaksymalnaTemperatura_1->text().toLatin1();
    TempMaxTemp_1.remove(-2,2);
    QString TempMaxTemp_2 = ui->MaksymalnaTemperatura_2->text().toLatin1();
    TempMaxTemp_2.remove(-2,2);
    QString TempMaxTemp_3 = ui->MaksymalnaTemperatura_3->text().toLatin1();
    TempMaxTemp_3.remove(-2,2);
    QString TempMaxCisnienie = ui->MaksymalneCisnienie->text().toLatin1();
    TempMaxCisnienie.remove(" psi");
    QString TempMaxMoment = ui->MaksymalnyMomet->text().toLatin1();
    TempMaxMoment.remove(" Nm");
    Wiadomosc.append(TempMaxTemp_1 + '!');
    Wiadomosc.append(TempMaxTemp_2 + '!');
    Wiadomosc.append(TempMaxTemp_3 + '!');
    Wiadomosc.append(TempMaxCisnienie+ '!');
    Wiadomosc.append(TempMaxMoment+ '!');
    if(stanPracySilnika){
       Wiadomosc.append("1!");
    }else Wiadomosc.append("0!");
    Wiadomosc.append(QString::number(ui->Obroty->value())+ '!');
    Wiadomosc.append(QString::number(ui->Grzalka_1->isChecked())+ '!');
    Wiadomosc.append(QString::number(ui->Grzalka_2->isChecked())+ '!');
    Wiadomosc.append(QString::number(ui->Grzalka_3->isChecked())+ '!');
    Wiadomosc.append(QString::number(ui->Chlodzenie_1->isChecked())+ '!');
    Wiadomosc.append(QString::number(ui->Chlodzenie_2->isChecked())+ '!');
    Wiadomosc.append(QString::number(ui->Chlodzenie_3->isChecked())+ '!');
    WyslijDoPortu(Wiadomosc);
}

void MainWindow::WyslijDoPortu(QString DaneDoWyslania)
{
    if(this->okno_polaczenie.Arduino->isOpen() &&
            this->okno_polaczenie.Arduino->isWritable())
    {
        qDebug() << "Wysyłam informacje do urządzenia " + DaneDoWyslania;
        this->okno_polaczenie.Arduino->write(DaneDoWyslania.toStdString().c_str());
    }
    else
    {
        qDebug() << DaneDoWyslania;
    }
}

void MainWindow::UstawTemperature()
{
    if(ui->UstawCisnienie->text() != NULL)
    {
        ui->MaksymalneCisnienie->
                setText(ui->UstawCisnienie->text()+" psi");
        ui->UstawCisnienie->clear();
    }
    if(ui->UstawMoment->text() != NULL)
    {
        ui->MaksymalnyMomet->
                setText(ui->UstawMoment->text()+" Nm");
        ui->UstawMoment->clear();
    }
    if(ui->UstawTemperature_1->text() != NULL)
    {
        ui->MaksymalnaTemperatura_1->
                setText(ui->UstawTemperature_1->text()+"°C");
        ui->UstawTemperature_1->clear();
    }
    if(ui->UstawTemperature_2->text() != NULL)
    {
        ui->MaksymalnaTemperatura_2->
                setText(ui->UstawTemperature_2->text()+"°C");
        ui->UstawTemperature_2->clear();
    }
    if(ui->UstawTemperature_3->text() != NULL)
    {
        ui->MaksymalnaTemperatura_3->
                setText(ui->UstawTemperature_3->text()+"°C");
        ui->UstawTemperature_3->clear();
    }
}

void MainWindow::ActionPolaczTriggered()
{
    okno_polaczenie.setModal(true);
    okno_polaczenie.exec();
}

void MainWindow::OdczytajZPortu()
{
    static int sekfencja;
    static QString Moment, Cisnienie, Temperatura_1, Temperatura_2, Temperatura_3, strGrzalka_1, strGrzalka_2, strGrzalka_3, strZawor_1, strZawor_2, strZawor_3;
    char DanaOdczytane[1000];
    QString Wiadomosc;
    switch (sekfencja) {
    case 0:
        if(!this->okno_polaczenie.Arduino->canReadLine())
        {
            return;
        }else
        {
            this->okno_polaczenie.Arduino->readLine(DanaOdczytane,30);
            Temperatura_1 = (QString)DanaOdczytane;
            sekfencja = 1;
        }
    case 1:
        if(!this->okno_polaczenie.Arduino->canReadLine())
        {
            return;
        }else
        {
            this->okno_polaczenie.Arduino->readLine(DanaOdczytane,30);
            Temperatura_2 = (QString)DanaOdczytane;
            sekfencja = 2;
        }
    case 2:
        if(!this->okno_polaczenie.Arduino->canReadLine())
        {
            return;
        }else
        {
            this->okno_polaczenie.Arduino->readLine(DanaOdczytane,30);
            Temperatura_3 = (QString)DanaOdczytane;
            sekfencja = 3;
        }
    case 3:
        if(!this->okno_polaczenie.Arduino->canReadLine())
        {
            return;
        }else
        {
            this->okno_polaczenie.Arduino->readLine(DanaOdczytane,30);
            Cisnienie = (QString)DanaOdczytane;
            sekfencja = 4;
        }
    case 4:
        if(!this->okno_polaczenie.Arduino->canReadLine())
        {
            return;
        }else
        {
            this->okno_polaczenie.Arduino->readLine(DanaOdczytane,30);
            Moment = (QString)DanaOdczytane;
            sekfencja = 5;
        }
    case 5:
        if(!this->okno_polaczenie.Arduino->canReadLine())
        {
            return;
        }else
        {
            this->okno_polaczenie.Arduino->readLine(DanaOdczytane,30);
            strGrzalka_1 = (QString)DanaOdczytane;
            sekfencja = 0;
        }
    case 6:
        if(!this->okno_polaczenie.Arduino->canReadLine())
        {
            return;
        }else
        {
            this->okno_polaczenie.Arduino->readLine(DanaOdczytane,30);
            strGrzalka_2 = (QString)DanaOdczytane;
            sekfencja = 0;
        }
    case 7:
        if(!this->okno_polaczenie.Arduino->canReadLine())
        {
            return;
        }else
        {
            this->okno_polaczenie.Arduino->readLine(DanaOdczytane,30);
            strGrzalka_3 = (QString)DanaOdczytane;
            sekfencja = 0;
        }
    case 8:
        if(!this->okno_polaczenie.Arduino->canReadLine())
        {
            return;
        }else
        {
            this->okno_polaczenie.Arduino->readLine(DanaOdczytane,30);
            strZawor_1 = (QString)DanaOdczytane;
            sekfencja = 0;
        }
    case 9:
        if(!this->okno_polaczenie.Arduino->canReadLine())
        {
            return;
        }else
        {
            this->okno_polaczenie.Arduino->readLine(DanaOdczytane,30);
            strZawor_2 = (QString)DanaOdczytane;
            sekfencja = 0;
        }
    case 10:
        if(!this->okno_polaczenie.Arduino->canReadLine())
        {
            return;
        }else
        {
            this->okno_polaczenie.Arduino->readLine(DanaOdczytane,30);
            strZawor_3 = (QString)DanaOdczytane;
            sekfencja = 0;
        }
    }


        Wiadomosc.append(czas+",");
        Wiadomosc.append(Temperatura_1);
        Wiadomosc.append(",");
        Wiadomosc.append(Temperatura_2);
        Wiadomosc.append(",");
        Wiadomosc.append(Temperatura_3);
        Wiadomosc.append(",");
        Wiadomosc.append(Cisnienie);
        Wiadomosc.append(",");
        Wiadomosc.append(Moment);
        Wiadomosc.append(",");
        Grzalka_1 = strGrzalka_1.toInt();
        Grzalka_2 = strGrzalka_2.toInt();
        Grzalka_3 = strGrzalka_3.toInt();
        Zawor_1 = strZawor_1.toInt();
        Zawor_2 = strZawor_2.toInt();
        Zawor_3 = strZawor_3.toInt();
        StanProtowWyswietl();
        Wiadomosc.replace("\r","", Qt::CaseSensitive);
        Wiadomosc.replace("\n","", Qt::CaseSensitive);
        Wiadomosc.append("\n");

        ZapisDoTymczasowegoPliku(Wiadomosc);

        this->okno_polaczenie.Arduino->clear();

        QString dispTemperatura_1 = Temperatura_1 + "°C";
        QString dispTemperatura_2 = Temperatura_2 + "°C";
        QString dispTemperatura_3 = Temperatura_3 + "°C";
        QString dispCisnienie = Cisnienie + " psi";
        QString dispMoment = Moment + " Nm";
        dispTemperatura_1.remove("\n");
        dispTemperatura_2.remove("\n");
        dispTemperatura_3.remove("\n");
        dispCisnienie.remove("\n");
        dispMoment.remove("\n");
        ui->AktualnaTemp_1->setText(dispTemperatura_1);
        ui->AktualnaTemp_2->setText(dispTemperatura_2);
        ui->AktualnaTemp_3->setText(dispTemperatura_3);
        ui->AktualneCisnienie->setText(dispCisnienie);
        ui->AktualnyMomet->setText(dispMoment);
}

void MainWindow::StanProtowWyswietl()
{
      if(Grzalka_1 == 1){
        ui->progressBarGrzalka_1->setValue(100);
      }else {
        ui->progressBarGrzalka_1->setValue(0);
      }
      if(Grzalka_2 == 1){
        ui->progressBarGrzalka_2->setValue(100);
      }else {
        ui->progressBarGrzalka_2->setValue(0);
      }
      if(Grzalka_3 == 1){
       ui->progressBarGrzalka_3->setValue(100);
      }else {
        ui->progressBarGrzalka_3->setValue(0);
      }
      if(Zawor_1 == 1){
        ui->progressBarChlodzenie_1  ->setValue(100);
      }else {
        ui->progressBarChlodzenie_1->setValue(0);
      }
      if(Zawor_2 == 1){
        ui->progressBarChlodzenie_2->setValue(100);
      }else{
        ui->progressBarChlodzenie_2->setValue(0);
      }
      if(Zawor_3 == 1){
        ui->progressBarChlodzenie_3->setValue(100);
      }else{
        ui->progressBarChlodzenie_3->setValue(0);
      }
}

void MainWindow::on_Ustaw_clicked()
{
     UstawTemperature();
     WyslanieRamkiDanych();
}


void MainWindow::ZapisDoTymczasowegoPliku(QString Tekst)
{
    CalyPlik.append(Tekst);
}

bool FirstcallresetUI = false;
void MainWindow::on_pushButton_clicked()
{
    if(FirstcallresetUI == false){
        timerresetUI->start(100);
        FirstcallresetUI = true;
    }
    CalyPlik = "Czas,Temperatura 1,Temperatura 2,Temperatura 3,Cisnienie,Moment\n";
    timer.restart();
}

void MainWindow::on_pushButton_2_clicked()
{
    QString FilePath = QFileDialog::getSaveFileName(this, tr("Zapisz Plik"),"Nowy Pomiar.csv",tr("Format CSV (*.csv *.CSV)"));
    QStringList FileNameList = FilePath.split("/");
    FilePath.remove(FileNameList.last());
    QDir dir(FilePath);
    QString FileName;
    if(FileNameList.last().contains(".csv")){
        FileName =  FileNameList.last();
    }else{
        FileName =  FileNameList.last() + ".csv";
    }
    QFile plikZapisu(dir.filePath(FileName));
    if (!plikZapisu.open(QIODevice::WriteOnly)){
         qDebug() << "cos nie halo";
        return;
    }
    qDebug() << CalyPlik;
    QTextStream out(&plikZapisu);
    out << CalyPlik;
    plikZapisu.close();
    qDebug() << "ok";
}


void MainWindow::resetUI(){

    QString millis = QString::number(timer.elapsed()%1000);
    QString sec = QString::number(timer.elapsed()/1000%60);
    QString min = QString::number(timer.elapsed()/1000/60%60);
    QString godz = QString::number(timer.elapsed()/1000/60/60);
    czas = godz +":"+ min +":"+ sec +":"+ millis;
    ui->PomiarCzasu->setText(czas);
}

void MainWindow::on_Grzalka_1_stateChanged()
{
    WyslanieRamkiDanych();
}

void MainWindow::on_Grzalka_2_stateChanged()
{
    WyslanieRamkiDanych();
}

void MainWindow::on_Grzalka_3_stateChanged()
{
    WyslanieRamkiDanych();
}

void MainWindow::on_Chlodzenie_1_stateChanged()
{
    WyslanieRamkiDanych();
}

void MainWindow::on_Chlodzenie_2_stateChanged()
{
    WyslanieRamkiDanych();
}

void MainWindow::on_Chlodzenie_3_stateChanged()
{
    WyslanieRamkiDanych();
}

void MainWindow::on_Obroty_sliderReleased()
{
    WyslanieRamkiDanych();
}

void MainWindow::on_Obroty_Prawe_pressed()
{
    stanPracySilnika = true;
    WyslanieRamkiDanych();
}

void MainWindow::on_Obroty_Lewe_pressed()
{
    stanPracySilnika = false;
    WyslanieRamkiDanych();
}

void MainWindow::on_Obroty_valueChanged()
{
    ui->ObrotyWartosc->setText(QString::number(ui->Obroty->value()*399/4095+1)+"Hz");
}
