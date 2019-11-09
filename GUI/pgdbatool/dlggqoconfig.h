#ifndef DLGGQOCONFIG_H
#define DLGGQOCONFIG_H

#include <QDialog>

namespace Ui {
class DlgGQOConfig;
}

class DlgGQOConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DlgGQOConfig(QWidget *parent = nullptr);
    ~DlgGQOConfig();

private:
    Ui::DlgGQOConfig *ui;
};

#endif // DLGGQOCONFIG_H
