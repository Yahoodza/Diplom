/**************************************************************************
*  MainWindow - модуль для взаимодействия с главным окном программы
***************************************************************************
*  Название: MainWindow.
*  Разработал: Королёв Петр Олегович, группа ТМП-81.
*  Язык программирования: C++.
*  Фреймворк: Qt.
*  Среда разработки: Qt Creator.
*  Краткое описание:
*       Данный модуль выполняет распаковку и выборку данных из массива
*       tmypl. Производит вывод и сохранение распакованных данных.
*
*  Имеющийся класс в модуле:
*       MainWindow - класс для распаковки, обработки, получения и
*       просмотра данных из файла tmypl.
*  Используемые методы:
*      closeEvent() - событие, возникающее при закрытии формы;
*      MainWindow() – конструктор класса;
*      ~MainWindow() – деструктор класса;
*      on_actionOpen_triggered() - выбор файла tmypl;
*      on_actionSD_triggered() - просмотр ИД;
*      on_actionFMNI_triggered() - просмотр ФМНИ;
*      on_actionSaveAll_triggered() - сохранение ИД и ФМНИ в заданную директорию;
*      on_actionSaveAll_AS_triggered() - сохранение ИД и ФМНИ с возможностью
*      выбора директории;
*      on_actionSaveSD_triggered() - сохранение ИД в заданную директорию;
*      on_actionSaveSD_AS_triggered() - сохранение ИД с возможностью
*      выбора директории;
*      on_actionSaveFMNI_triggered() - сохранение ФМНИ в заданную директорию;
*      on_actionSaveFMNI_AS_triggered() - сохранение ФМНИ с возможностью
*      выбора директории;
*      TmyplSession() - создание таблицы сеансов tmypl;
*      on_tableWidget_itemDoubleClicked() - просмотр ФМНИ выбранного
*      сеанса в виде таблицы;
*      on_actionReturn_triggered() - возврат к таблице выбора сеанса;
*      turnOver() - переворот сеанса ИИ побайтово;
*      deleteMG() - удаление МГ сеанса и подсчёт Мнди и Мн сеанса;
*      quantityFormular() - расчёт количества ФМНИ в сеансе;
*      on_pushButtonSaveOut_clicked() - сохранение выходного набора
*      с расширением .bin и .txt в заданную директорию;
*      on_pushButtonOpenFileOut_clicked() - распаковка файла выходного набора и вывод
*      данных на экран;
*      on_pushButtonViewOut_clicked() - предпросмотр выходного набора сеанса;
*      on_actionSetup_triggered() - открытие окна настроек сохранения
*      файлов приложения;
*      on_actionGraph_triggered() - открытие окна построения графиков
*      расхождений и вывода таблиц расчётов;
*      on_actionWhite_triggered() - изменение темы приложения на светлую;
*      on_actionDark_triggered() - изменение темы приложения на тёмную.
*  Используемые переменные:
*      SetupWindow - окно настроек;
*      GraphWindow - окно построения графика расхождений и таблиц расчётов;
*      Filename - имя файла вида tmypl;
*      FileSize - размер в байтах файла вида tmypl;
*      textSD - информация из ИД с пояснением каждого параметра;
*      textMNI - информация из МНИ с пояснением каждого параметра;
*      DataLoc - структура SMID;
*      DataRem - структура ID0;
*      ArrRef - структура MSI;
*      CMarr - массив, содержащий информацию о расположении сеансов
*      ИД из структуры MSI;
*      Session - структура ID1;
*      ID1ses - массив ИД по сеансам включений из структуры ID1;
*      SpravOSprav - структура SMsp_type;
*      Sprav - структура sSPRAV_type;
*      SesSprav - массив справки о положении сеансов наблюдения из
*      структуры sSPRAV_type;
*      Start - массив смещений от начала файла до начала сеанса ИИ;
*      End - массив смещений от начала файла до конца сеанса ИИ;
*      BufferSeans - массив данных из сеансов ИИ;
*      FMNI - структура ФМНИ;
*      FMNIses - двумерный массив данных ФМНИ по сеансам;
*      ComboIndex - выбранный сеанс выходного набора;
*      Mndi - подсчёт Мнди во всей ИИ;
*      Mn - подсчёт Мн во всей ИИ;
*      MndiSes - массив количества Мнди в сеансе ИИ;
*      MnSes - массив количества Мн в сеансе ИИ;
*      Output - структура выходного набора;
*      ProgressDialog - окно загрузки приложения;
*      SetSave - подключение к файлу настроек.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "Structs.h"
#include <QDataStream>
#include <QStandardPaths>
#include <QtCore/qmath.h>
#include <QMessageBox>
#include <QTableWidget>
#include <QProgressDialog>
#include <setupdialog.h>
#include <graphdialog.h>
#include <QSettings>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionFMNI_triggered();

    void on_actionSD_triggered();

    void on_actionSaveAll_triggered();

    void on_actionSaveAll_AS_triggered();

    void on_actionSaveSD_triggered();

    void on_actionSaveSD_AS_triggered();

    void on_actionSaveFMNI_triggered();

    void on_actionSaveFMNI_AS_triggered();
    
    void TmyplSession(unsigned int, unsigned long, unsigned long);    

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_actionReturn_triggered();

    void turnOver(unsigned int);

    QVector<int> deleteMG(unsigned int);

    unsigned long quantityFormular(unsigned int);

    void on_pushButtonSaveOut_clicked();

    void on_pushButtonOpenFileOut_clicked();

    void on_pushButtonViewOut_clicked();

    void on_actionSetup_triggered();

    void on_actionGraph_triggered();

    void on_actionWhite_triggered();

    void on_actionDark_triggered();

protected:
    virtual void closeEvent(QCloseEvent *);

private:
    Ui::MainWindow *ui;
    SetupDialog *SetupWindow;
    GraphDialog *GraphWindow;

    QString Filename;
    int FileSize;
    QString textSD, textMNI;
    SMID *DataLoc = new SMID();
    ID0 *DataRem = new ID0();
    MSI *ArrRef = new MSI();
    QVector<MSI> CMarr;
    ID1 *Session = new ID1();
    QVector<ID1> ID1ses;
    SMsp_type *SpravOSprav = new SMsp_type();
    sSPRAV_type *Sprav = new sSPRAV_type();
    QVector<sSPRAV_type> SesSprav;

    QVector<long> Start;
    QVector<long> End;
    QVector<QByteArray> BufferSeans;
    MNI_type *FMNI = new MNI_type();
    QVector<QVector<MNI_type>> FMNIses;

    short ComboIndex;
    long Mndi, Mn;
    QVector<long> MndiSes, MnSes;
    OutputSet_type *Output = new OutputSet_type();
    QVector<QVector<QVector<long>>> TableCalcSD;

    QProgressDialog *ProgressDialog;
    QSettings *SetSave = new QSettings
            (QCoreApplication::applicationDirPath() + "/Settings/SettingsNIprogram.ini",
             QSettings::IniFormat);
};
#endif // MAINWINDOW_H
