#ifndef SETUPFORM_H
#define SETUPFORM_H

#include <QDialog>
#include <QFileDialog>
#include <QSettings>
#include <QCoreApplication>

namespace Ui {
class SetupForm;
}

class SetupForm : public QDialog
{
    Q_OBJECT

public:
    explicit SetupForm(QDialog *parent = nullptr);
    ~SetupForm();

private slots:
    void on_lineEditAll_editingFinished();

    void on_pushButtonAll_clicked();

    void on_lineEditSD_editingFinished();

    void on_pushButtonSD_clicked();

    void on_lineEditMNI_editingFinished();

    void on_pushButtonMNI_clicked();

    void on_lineEditOut_editingFinished();

    void on_pushButtonOut_clicked();

    void on_pushButtonDefault_clicked();

private:
    Ui::SetupForm *ui;
    QSettings *Set = new QSettings(QCoreApplication::applicationDirPath() + "/Settings/SettingsNIprogram.ini", QSettings::IniFormat);

};

#endif // SETUPFORM_H
