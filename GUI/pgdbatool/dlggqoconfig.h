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

    void setEnableGEQO(bool enable);
    void setThreshold(int value);
    void setEffort(int value);
    void setPoolSize(int value);
    void setGenerations(int value);
    void setSelectionBias(double value);
    void setSeed(double seed);

    bool getEnableGEQO();
    int getThreshold();
    int getEffort();
    int getPoolSize();
    int getGenerations();
    double getSelectionBias();
    double getSeed();

private:
    Ui::DlgGQOConfig *ui;
};

#endif // DLGGQOCONFIG_H
