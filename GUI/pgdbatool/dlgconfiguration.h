#ifndef DLGCONFIGURATION_H
#define DLGCONFIGURATION_H

#include <QDialog>

namespace Ui {
class DlgConfiguration;
}

class DlgConfiguration : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConfiguration(QWidget *parent = nullptr);
    ~DlgConfiguration();
    QString getPathToModels();
    QString getPathToSql();
    void setPathToModels(QString value);
    void setPathToSql(QString value);
private slots:
    void on_bt_open_path_models_clicked();
    void on_bt_open_path_sql_clicked();

private:
    Ui::DlgConfiguration *ui;
};

#endif // DLGCONFIGURATION_H
