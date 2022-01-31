//#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "polaczenie.h"
#include <QTimer>
#include <QLabel>
#include <QProgressBar>
#include <QSerialPort>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSerialPort *Arduino;
    QTimer *timerresetUI;
    int progres = 0;
    Polaczenie okno_polaczenie;
    bool stanPracySilnika = false;
    QElapsedTimer timer;
    QString czas;
    QString CalyPlik;
    int Stangrzalka_1;
    int Stangrzalka_2;
    int Stangrzalka_3;
    int Stanzawor_1;
    int Stanzawor_2;
    int Stanzawor_3;
    int Grzalka_1;
    int Grzalka_2;
    int Grzalka_3;
    int Zawor_1;
    int Zawor_2;
    int Zawor_3;
    int skalaCianienie=1;
    int skalaMoment=1;

private slots:
    void on_Ustaw_clicked();
    void UstawTemperature();
    void ActionPolaczTriggered();
    void OdczytajZPortu();
    void WyslijDoPortu(QString DaneDoWyslania);
    void ZapisDoTymczasowegoPliku(QString Tekst);
    void on_Obroty_Lewe_pressed();
    void on_Obroty_Prawe_pressed();
    void WyslanieRamkiDanych();
    void on_Grzalka_1_stateChanged();
    void on_Grzalka_2_stateChanged();
    void on_Grzalka_3_stateChanged();
    void on_Chlodzenie_1_stateChanged();
    void on_Chlodzenie_2_stateChanged();
    void on_Chlodzenie_3_stateChanged();
    void StanProtowWyswietl();
    void on_pushButton_2_clicked();
    void on_Obroty_sliderReleased();
    void on_pushButton_clicked();
    void resetUI();
    void on_Obroty_valueChanged();

private:
    Ui::MainWindow *ui;
    QLabel *AlarmLabel;
    QProgressBar *statusProgressBar;
};

