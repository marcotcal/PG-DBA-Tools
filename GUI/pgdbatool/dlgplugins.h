#ifndef DLGPLUGINS_H
#define DLGPLUGINS_H

#include <QDialog>
#include <pgdbaplugininterface.h>

namespace Ui {
class DlgPlugins;
}

class DlgPlugins : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPlugins(QMap<QString, PGDBAPluginInterface *> &interface_list, QWidget *parent = 0);
    ~DlgPlugins();

private slots:
    void on_bt_close_clicked();

private:
    Ui::DlgPlugins *ui;
};

#endif // DLGPLUGINS_H
