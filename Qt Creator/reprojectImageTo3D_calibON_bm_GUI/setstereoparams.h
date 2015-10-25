#ifndef SETSTEREOPARAMS_H
#define SETSTEREOPARAMS_H

#include <QDialog>

namespace Ui {
class SetStereoParams;
}

class SetStereoParams : public QDialog
{
    Q_OBJECT

public:
    explicit SetStereoParams(QWidget *parent = 0);
    ~SetStereoParams();

private slots:

private:
    Ui::SetStereoParams *ui;
};

#endif // SETSTEREOPARAMS_H
