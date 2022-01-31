#include "narzedzia.h"
#include "ui_narzedzia.h"

Narzedzia::Narzedzia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Narzedzia)
{
    ui->setupUi(this);
}

Narzedzia::~Narzedzia()
{
    delete ui;
}
