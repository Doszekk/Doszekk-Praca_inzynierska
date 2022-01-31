#include "export.h"
#include "ui_export.h"
#include "QString"
#include "QList"
#include "QFile"
#include "QTextStream"
#include "QDir"
#include "QDebug"






using namespace std;

Export::Export(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Export)
{
    ui->setupUi(this);
}

Export::~Export()
{
    delete ui;
}


void Export::ZapisDoFormatuCSV()
{
    QDir dir1(QDir::homePath());
    QFile plik1(dir1.filePath("ex1.txt"));
    if (!plik1.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "coś ne bardzo";
        return;
    }

    QTextStream in(&plik1);
    QString CalyPlik = in.readAll();
    plik1.close();
    qDebug() << "przynajniej tutaj";
    QDir dir2(ui->SciezkaDostepu_LineEdit->text());
    qDebug() << CalyPlik;
    QFile plik2(dir2.filePath(ui->NazwaPliku_LineEdit->text()+ui->comboBox->currentText()));
    if (!plik2.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
        qDebug() << "coś ne bardzo";
        return;
    }
    qDebug() << "przynajniej tutaj";
    QTextStream out(&plik2);
    out << CalyPlik;
    plik2.close();
    qDebug() << CalyPlik;
}

void Export::on_pushButton_clicked()
{
    ZapisDoFormatuCSV();
}

