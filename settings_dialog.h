#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    int getInterval() const;
    void setInterval(int interval);

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGS_DIALOG_H
