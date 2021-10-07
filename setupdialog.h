/**************************************************************************
*  SetupDialog - модуль для взаимодействия с окном настроек
***************************************************************************
*  Название: SetupDialog.
*  Разработал: Королёв Петр Олегович, группа ТМП-81.
*  Язык программирвоания: C++.
*  Фреймворк: Qt.
*  Среда разработки: Qt Creator.
*  Краткое описание:
*      Данный модуль изменяет стандартные настройки сохранения файлов программы
*      или возвращает изменения по умолчанию.
*
*  Имеющийся класс в модуле:
*      SetupDialog - класс для изменения настроек сохранения файлов программы.
*  Используемые методы:
*      SetupDialog() – конструктор класса;
*      ~SetupDialog() – деструктор класса;
*      on_lineEditAll_returnPressed() - изменение пути сохранения ИД и
*      ФМНИ вручную;
*      on_pushButtonAll_clicked() - изменение пути сохранения ИД и
*      ФМНИ через проводник;
*      on_lineEditSD_returnPressed() - изменение пути сохранения ИД вручную;
*      on_pushButtonSD_clicked() - изменение пути сохранения ИД через проводник;
*      on_lineEditMNI_returnPressed() - изменение пути сохранения ФМНИ вручную;
*      on_pushButtonMNI_clicked() - изменение пути сохранения ФМНИ через
*      проводник;
*      on_lineEditOut_returnPressed() - изменение пути сохранения выходного
*      набора вручную;
*      on_pushButtonOut_clicked() - изменение пути сохранения выходного набора
*      через проводник;
*      on_lineEditAllGraph_returnPressed() - изменение пути сохранения графиков
*      вручную;
*      on_pushButtonAllGraph_clicked() - изменение пути сохранения графиков
*      через проводник;
*      on_lineEditAllTable_returnPressed() - изменение пути сохранения всех таблиц
*      расчётов вручную;
*      on_pushButtonAllTable_clicked() - изменение пути сохранения всех таблиц
*      расчётов через проводник;
*      on_lineEditSDTable_returnPressed() - изменение пути сохранения таблицы
*      расчётов по ИД вручную;
*      on_pushButtonSDTable_clicked() - изменение пути сохранения таблицы
*      расчётов по ИД через проводник;
*      on_lineEditMNITable_returnPressed() - изменение пути сохранения таблицы
*      расчётов по МНИ вручную;
*      on_pushButtonMNITable_clicked() - изменение пути сохранения таблицы
*      расчётов по МНИ через проводник;
*      on_lineEditDivergTable_returnPressed() - изменение пути сохранения таблицы
*      отклонений орбит вручную;
*      on_pushButtonDivergTable_clicked() - изменение пути сохранения таблицы
*      отклонений орбит через проводник;
*      on_pushButtonDefault_clicked() - возвращение к настройкам по умолчанию.
*  Используемая переменная:
*      Set - подключение к файлу настроек.
*/

#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QSettings>
#include <QCoreApplication>
#include <QMessageBox>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDialog(QWidget *parent = nullptr);
    ~SetupDialog();

private slots:
    void on_lineEditAll_returnPressed();

    void on_pushButtonAll_clicked();

    void on_lineEditSD_returnPressed();

    void on_pushButtonSD_clicked();

    void on_lineEditMNI_returnPressed();

    void on_pushButtonMNI_clicked();

    void on_lineEditOut_returnPressed();

    void on_pushButtonOut_clicked();

    void on_lineEditAllGraph_returnPressed();

    void on_pushButtonAllGraph_clicked();

    void on_lineEditAllTable_returnPressed();

    void on_pushButtonAllTable_clicked();

    void on_lineEditSDTable_returnPressed();

    void on_pushButtonSDTable_clicked();

    void on_lineEditMNITable_returnPressed();

    void on_pushButtonMNITable_clicked();

    void on_lineEditDivergTable_returnPressed();

    void on_pushButtonDivergTable_clicked();

    void on_pushButtonDefault_clicked();

private:
    Ui::SetupDialog *ui;
    QSettings *Set = new QSettings
            (QCoreApplication::applicationDirPath() + "/Settings/SettingsNIprogram.ini",
             QSettings::IniFormat);

};

#endif // SETUPDIALOG_H
