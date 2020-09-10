#ifndef DLGPLUGINS_H
#define DLGPLUGINS_H

#include <QDialog>
#include <pgdbaplugininterface.h>
#include <sqltool.h>
#include <pluginelement.h>

namespace Ui {
class DlgPlugins;
}

class DlgPlugins : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPlugins(QMap<QString, PluginElement *> &interface_list, QWidget *parent = 0);
    ~DlgPlugins();

    void setEditor(EditorItem *value);
private slots:

    void on_bt_close_clicked();
    void on_bt_test_clicked();

private:
    Ui::DlgPlugins *ui;
    QMap<QString, PluginElement *> &interface_list;
    EditorItem *editor;

    void loadTable();
};

#endif // DLGPLUGINS_H
