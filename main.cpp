/**************************************************************************
*  Дипломный проект на тему "Программа определения расхождения реальной
*  орбиты движения центра масс космического аппарата с прогнозируемой
*  в процессе мониторинга поверхности Земли"
***************************************************************************
*  Название: Navigation Information.
*  Разработал: Королёв Петр Олегович, группа ТМП-81.
*  Специальность: 09.02.03.
*  Дата и номер версии: 1.06.2020 v1.5.
*  Язык программирования: C++.
*  Фреймворк: Qt.
*  Среда разработки: Qt Creator.
*  Краткое описание:
*       Данная программа определяет расхождение реальной орбиты движения КА
*       с прогнозируемой и на основе этих данных строит график.
*
*  Задание:
*      Реализовать распаковку данных из файла вида tmypl с последующим сохранением
*      этих данных в удобной для заказчика форме. Необходимо разработать
*      программу, которая позволит наглядно продемонстрировать реальное положение
*      КА на орбите Земли с прогнозируемым.
*
*  Программа должна обеспечивать возможность выполнения следующих функций:
*       - проверка файла, является ли он информационным массивом tmypl;
*       - распаковка информации из массива tmypl;
*       - вывод на экран и сохранение ИД в текстовый файл;
*       - вывод на экран и сохранение ФМНИ в текстовый файл;
*       - сохранение выходного набора в файл с расширением .bin и .txt;
*       - распаковка выходного набора из файла;
*       - настройка путей сохранения файлов программы;
*       - отображение расхождений реальной и прогнозируемой орбит движения
*       КА в виде графика и предоставление таблиц расчётов с возможность их
*       сохранения.
*
*  Ожидаемые входные данные:
*       1) бинарный файл информационного массива tmypl;
*       2) пути сохранения файлов.
*
* Используемая функция:
*       main() - главная функция программы, открывает основное окно приложения.
*
*  Используемые модули в программе:
*       MainWindow - модуль для взаимодействия с главным окном программы;
*       Structs - модуль, содержащий структуры информационного массива tmypl;
*       SetupDialog - модуль для взаимодействия с окном настроек;
*       GraphDialog - модуль для взаимодействия с окном построения графика
*       расхождений и таблиц расчётов.
*/


#include "MainWindow.h"
#include <QApplication>

/* main() - главная функция программы, открывает основное окно приложения.
*  Формальные параметры:
*       argc - количество переданных параметров при
*       запуске исполняемого файла;
*       argv - массив указателей на переданные параметры
*       при запуске исполняемого файла.
* Локальные переменные:
*       dir - путь создания директории для хранения файла настроек;
*       Settings - создание файла настроек формата ini;
*       dirFile - путь стандартного сохранения выходных файлов программы;
*       a - объект класса, контролирующий главный поток и основные
*       настройки ПО;
*       w - основное окно, предоставляющее библиотеки для создания
*       пользовательского интерфейса.
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QDir dir(QCoreApplication::applicationDirPath());
    dir.mkpath("Settings");                                                         // Проверка существования директории
    QSettings Settings(QCoreApplication::applicationDirPath() + "/Settings/SettingsNIprogram.ini", QSettings::IniFormat);
                                                                                    // Создание файла настроек
    if (!Settings.childGroups().contains("default", Qt::CaseInsensitive) ||
        !Settings.childGroups().contains("changed", Qt::CaseInsensitive))           // Условие проверки существования настроек
    {
        Settings.clear();
        QDir dirFile(QDir::homePath() + "/Downloads/");
        dirFile.mkpath("Tmypl Info");
        Settings.setValue("default/dir", dirFile.absolutePath() + "/Tmypl Info");
        Settings.setValue("default/dirHomePath", dirFile.absolutePath());
        Settings.setValue("default/dirName", "Tmypl Info");
        Settings.setValue("changed/dirAll", dirFile.absolutePath() + "/Tmypl Info");
        Settings.setValue("changed/dirSD", dirFile.absolutePath() + "/Tmypl Info");
        Settings.setValue("changed/dirMNI", dirFile.absolutePath() + "/Tmypl Info");
        Settings.setValue("changed/dirOut", dirFile.absolutePath() + "/Tmypl Info");
        Settings.setValue("changed/dirAllGraph", dirFile.absolutePath() + "/Tmypl Info");
        Settings.setValue("changed/dirAllTable", dirFile.absolutePath() + "/Tmypl Info");
        Settings.setValue("changed/dirSDTable", dirFile.absolutePath() + "/Tmypl Info");
        Settings.setValue("changed/dirMNITable", dirFile.absolutePath() + "/Tmypl Info");
        Settings.setValue("changed/dirDivergTable", dirFile.absolutePath() + "/Tmypl Info");
        Settings.setValue("changed/w_or_d", "w");
    }

    w.showMaximized();
    return a.exec();
}
