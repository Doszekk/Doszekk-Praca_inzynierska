#ifndef NARZEDZIA_H
#define NARZEDZIA_H

#include <QDialog>

namespace Ui {
class Narzedzia;
}

class Narzedzia : public QDialog
{
    Q_OBJECT

public:
    explicit Narzedzia(QWidget *parent = nullptr);
    ~Narzedzia();

private:
    Ui::Narzedzia *ui;
};

#endif // NARZEDZIA_H
