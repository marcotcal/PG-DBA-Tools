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

    enum {
        SCG_INCLUDE_TASK_CODE = 1,
        SCG_INCLUDE_SPRINT_CODE = 2,
        SCG_KEEP_ON_REVIEW = 4,
        SCG_SERIALIZE_BY_DATE = 8,
        SCG_SERIALIZE_BY_INTEGER = 16
    };

    explicit DlgConfiguration(bool ready_only = false, QWidget *parent = nullptr);
    ~DlgConfiguration();

    QString getPathToModels();
    QString getPathToSql();
    int getOptions();
    QString getTaskPrefix();

    void setPathToModels(QString value);
    void setPathToSql(QString value);
    void setOptions(int value);
    void setTaskPrefix(QString value);

private slots:
    void on_bt_open_path_models_clicked();
    void on_bt_open_path_sql_clicked();

private:
    Ui::DlgConfiguration *ui;
};

#endif // DLGCONFIGURATION_H
