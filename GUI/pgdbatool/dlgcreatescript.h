#ifndef DLGCREATESCRIPT_H
#define DLGCREATESCRIPT_H

#include <QDialog>

namespace Ui {
class DlgCreateScript;
}

class DlgCreateScript : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCreateScript(QWidget *parent = nullptr);
    ~DlgCreateScript();

private:
    Ui::DlgCreateScript *ui;
};

#endif // DLGCREATESCRIPT_H
