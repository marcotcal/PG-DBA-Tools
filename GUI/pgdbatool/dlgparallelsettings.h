#ifndef DLGPARALLELSETTINGS_H
#define DLGPARALLELSETTINGS_H

#include <QDialog>

namespace Ui {
class DlgParallelSettings;
}

class DlgParallelSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DlgParallelSettings(QWidget *parent = nullptr);
    ~DlgParallelSettings();

private:
    Ui::DlgParallelSettings *ui;
};

#endif // DLGPARALLELSETTINGS_H
