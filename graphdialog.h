/**************************************************************************
*  GraphDialog - модуль построения графиков расхождений
***************************************************************************
*  Название: GraphDialog.
*  Разработал: Королёв Петр Олегович, группа ТМП-81.
*  Язык программирвоания: C++.
*  Фреймворк: Qt.
*  Среда разработки: Qt Creator.
*  Краткое описание:
*      Данный модуль производит вычисления реальных и прогнозируемых координат
*      X, Y и Z, заносит эти данные в таблицу и на основе этх координат строит
*      графики расхождений по осям.
*
*  Имеющийся класс в модуле:
*      GraphDialog - класс для расчёта таблиц расходений и построения графика.
*  Используемые методы:
*      GraphDialog() - конструктор класса;
*      ~GraphDialog() – деструктор класса;
*      changedWhiteTheame() - устанавливает светлую тему графиков;
*      changedDarkTheame() - устанавливает тёмную тему графиков;
*      on_comboBox_currentIndexChanged() - событие изменения сеанса;
*      on_pushButtonSD_clicked() - сохранение таблицы расчётов по ИД;
*      on_pushButtonMNI_clicked() - сохранение таблицы расчётов по МНИ;
*      on_pushButtonDiverg_clicked() - сохранение таблицы отклонений орбит;
*      on_pushButtonAllTable_clicked() - сохранение всех таблиц расчётов;
*      on_pushButtonGraph_clicked() - сохранение графиков расхождений.
*  Используемые переменные:
*      CalcSes - трёхмерный массив для расчёта координат каждого формуляра
*      МНИ по сеансам;
*      TableSD - таблица расчётов по ИД;
*      TableMNI - таблица расчётов по МНИ;
*      TableDiverg - таблица отклонений орбит;
*      Set - подключение к файлу настроек.
*/

#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include <QDialog>
#include <Structs.h>
#include <QtCore/qmath.h>
#include <qcustomplot.h>

namespace Ui {
class GraphDialog;
}

class GraphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GraphDialog(SMID *DataLoc, QVector<ID1> *ID1ses, QVector<QVector<MNI_type>> *FMNIses, QWidget *parent = nullptr);
    ~GraphDialog();

    void changedWhiteTheame();

    void changedDarkTheame();

private slots:
    void on_comboBox_currentIndexChanged(int index);
    
    void on_pushButtonSD_clicked();

    void on_pushButtonMNI_clicked();

    void on_pushButtonDiverg_clicked();

    void on_pushButtonAllTable_clicked();

    void on_pushButtonGraph_clicked();

private:
    Ui::GraphDialog *ui;
    QVector<QVector<QVector<double>>>  CalcSes;
    QString TableSD, TableMNI, TableDiverg;

    QSettings *Set = new QSettings
            (QCoreApplication::applicationDirPath() + "/Settings/SettingsNIprogram.ini",
             QSettings::IniFormat);
};

#endif // GRAPHDIALOG_H
