// SetupDialog - модуль для взаимодействия с окном настроек

#include "setupdialog.h"
#include "ui_setupdialog.h"

/* SetupDialog() – конструктор класса.
*  Формальный параметр:
*       parent - объект для отображения интерфейса.
*/

SetupDialog::SetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupDialog)
{
    ui->setupUi(this);
    setWindowTitle("Настройки");
    this->setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint |
                         Qt::WindowCloseButtonHint);

    ui->tabWidget->setTabText(0, "Настройки главного окна");
    ui->tabWidget->setTabText(1, "Настройки окна графиков и таблиц");

    ui->lineEditAll->setText(Set->value("changed/dirAll").toString());
    ui->lineEditSD->setText(Set->value("changed/dirSD").toString());
    ui->lineEditMNI->setText(Set->value("changed/dirMNI").toString());
    ui->lineEditOut->setText(Set->value("changed/dirOut").toString());
    ui->lineEditAllGraph->setText(Set->value("changed/dirAllGraph").toString());
    ui->lineEditAllTable->setText(Set->value("changed/dirAllTable").toString());
    ui->lineEditSDTable->setText(Set->value("changed/dirSDTable").toString());
    ui->lineEditMNITable->setText(Set->value("changed/dirMNITable").toString());
    ui->lineEditDivergTable->setText(Set->value("changed/dirDivergTable").toString());
}

//  ~SetupDialog() – деструктор класса.

SetupDialog::~SetupDialog()
{
    delete ui;
}

// on_lineEditAll_returnPressed() - изменение пути сохранения ИД и ФМНИ вручную.

void SetupDialog::on_lineEditAll_returnPressed()
{
    if (QDir(ui->lineEditAll->text()).mkpath(".") == false)
    {                                       // Проверка существования директории
        ui->lineEditAll->setText(Set->value("changed/dirAll").toString());
        QMessageBox::critical
                (this, "Ошибка", "Неверный путь!\n\nИмя файла не должно содержать следующих знаков:\n: * ? \" < > |");
        return;
    }
    ui->lineEditAll->setText(QDir(ui->lineEditAll->text()).absolutePath());
    Set->setValue("changed/dirAll", ui->lineEditAll->text());
    QMessageBox::information(this, "Изменение", "Путь к директории\nуспешно изменён!");
}

/* on_pushButtonAll_clicked() - изменение пути сохранения ИД и ФМНИ
*  через проводник.
*  Локальная переменная:
*       selDirAll - изменённый путь.
*/

void SetupDialog::on_pushButtonAll_clicked()
{
    QString selDirAll = QFileDialog::getExistingDirectory
            (this,tr("Выберите директорию для сохранения всего"));
    if (selDirAll.trimmed() != "")
    {                                       // Проверка на выбор новой директории сохранения
        ui->lineEditAll->setText(selDirAll);
        on_lineEditAll_returnPressed();
    }
}

// on_lineEditSD_returnPressed() - изменение пути сохранения ИД вручную.

void SetupDialog::on_lineEditSD_returnPressed()
{
    if (QDir(ui->lineEditSD->text()).mkpath(".") == false)
    {                                       // Проверка существования директории
        ui->lineEditSD->setText(Set->value("changed/dirSD").toString());
        QMessageBox::critical
                (this, "Ошибка", "Неверный путь!\n\nИмя файла не должно содержать следующих знаков:\n: * ? \" < > |");
        return;
    }
    ui->lineEditSD->setText(QDir(ui->lineEditSD->text()).absolutePath());
    Set->setValue("changed/dirSD", ui->lineEditSD->text());
    QMessageBox::information(this, "Изменение", "Путь к директории\nуспешно изменён!");
}

/* on_pushButtonSD_clicked() - изменение пути сохранения ИД через проводник.
*  Локальная переменная:
*       selDirSD - изменённый путь.
*/

void SetupDialog::on_pushButtonSD_clicked()
{
    QString selDirSD = QFileDialog::getExistingDirectory
            (this,tr("Выберите директорию для сохранения ИД"));
    if (selDirSD.trimmed() != "")
    {                                       // Проверка на выбор новой директории сохранения
        ui->lineEditSD->setText(selDirSD);
        on_lineEditSD_returnPressed();
    }
}

// on_lineEditMNI_returnPressed() - изменение пути сохранения ФМНИ вручную.

void SetupDialog::on_lineEditMNI_returnPressed()
{
    if (QDir(ui->lineEditMNI->text()).mkpath(".") == false)
    {                                       // Проверка существования директории
        ui->lineEditMNI->setText(Set->value("changed/dirMNI").toString());
        QMessageBox::critical
                (this, "Ошибка", "Неверный путь!\n\nИмя файла не должно содержать следующих знаков:\n: * ? \" < > |");
        return;
    }
    ui->lineEditMNI->setText(QDir(ui->lineEditMNI->text()).absolutePath());
    Set->setValue("changed/dirMNI", ui->lineEditMNI->text());
    QMessageBox::information(this, "Изменение", "Путь к директории\nуспешно изменён!");
}

/* on_pushButtonMNI_clicked() - изменение пути сохранения ФМНИ
*  через проводник.
*  Локальная переменная:
*       selDirMNI - изменённый путь.
*/

void SetupDialog::on_pushButtonMNI_clicked()
{
    QString selDirMNI = QFileDialog::getExistingDirectory
            (this,tr("Выберите директорию для сохранения ФМНИ"));
    if (selDirMNI.trimmed() != "")
    {                                       // Проверка на выбор новой директории сохранения
        ui->lineEditMNI->setText(selDirMNI);
        on_lineEditMNI_returnPressed();
    }
}

/* on_lineEditOut_returnPressed() - изменение пути сохранения выходного
*  набора вручную.
*/

void SetupDialog::on_lineEditOut_returnPressed()
{
    if (QDir(ui->lineEditOut->text()).mkpath(".") == false)
    {                                       // Проверка существования директории
        ui->lineEditOut->setText(Set->value("changed/dirOut").toString());
        QMessageBox::critical
                (this, "Ошибка", "Неверный путь!\n\nИмя файла не должно содержать следующих знаков:\n: * ? \" < > |");
        return;
    }
    ui->lineEditOut->setText(QDir(ui->lineEditOut->text()).absolutePath());
    Set->setValue("changed/dirOut", ui->lineEditOut->text());
    QMessageBox::information(this, "Изменение", "Путь к директории\nуспешно изменён!");
}

/* on_pushButtonOut_clicked() - изменение пути сохранения выходного набора
*  через проводник.
*  Локальная переменная:
*       selDirOut - изменённый путь.
*/

void SetupDialog::on_pushButtonOut_clicked()
{
    QString selDirOut = QFileDialog::getExistingDirectory
            (this, tr("Выберите директорию для сохранения выходного набора"));
    if (selDirOut.trimmed() != "")
    {                                       // Проверка на выбор новой директории сохранения
        ui->lineEditOut->setText(selDirOut);
        on_lineEditOut_returnPressed();
    }
}

/* on_lineEditAllGraph_returnPressed() - изменение пути сохранения графиков
*  вручную.
*/

void SetupDialog::on_lineEditAllGraph_returnPressed()
{
    if (QDir(ui->lineEditAllGraph->text()).mkpath(".") == false)
    {                                       // Проверка существования директории
        ui->lineEditAllGraph->setText(Set->value("changed/dirAllGraph").toString());
        QMessageBox::critical
                (this, "Ошибка", "Неверный путь!\n\nИмя файла не должно содержать следующих знаков:\n: * ? \" < > |");
        return;
    }
    ui->lineEditAllGraph->setText(QDir(ui->lineEditAllGraph->text()).absolutePath());
    Set->setValue("changed/dirAllGraph", ui->lineEditAllGraph->text());
    QMessageBox::information(this, "Изменение", "Путь к директории\nуспешно изменён!");
}

/* on_pushButtonAllGraph_clicked() - изменение пути сохранения графиков
*  через проводник.
*  Локальная переменная:
*       selDirAllGraph - изменённый путь.
*/

void SetupDialog::on_pushButtonAllGraph_clicked()
{
    QString selDirAllGraph = QFileDialog::getExistingDirectory
            (this, tr("Выберите директорию для сохранения графиков"));
    if (selDirAllGraph.trimmed() != "")
    {                                       // Проверка на выбор новой директории сохранения
        ui->lineEditAllGraph->setText(selDirAllGraph);
        on_lineEditAllGraph_returnPressed();
    }
}

/* on_lineEditAllTable_returnPressed() - изменение пути сохранения всех таблиц
*  расчётов вручную.
*/

void SetupDialog::on_lineEditAllTable_returnPressed()
{
    if (QDir(ui->lineEditAllTable->text()).mkpath(".") == false)
    {                                       // Проверка существования директории
        ui->lineEditAllTable->setText(Set->value("changed/dirAllTable").toString());
        QMessageBox::critical
                (this, "Ошибка", "Неверный путь!\n\nИмя файла не должно содержать следующих знаков:\n: * ? \" < > |");
        return;
    }
    ui->lineEditAllTable->setText(QDir(ui->lineEditAllTable->text()).absolutePath());
    Set->setValue("changed/dirAllTable", ui->lineEditAllTable->text());
    QMessageBox::information(this, "Изменение", "Путь к директории\nуспешно изменён!");
}

/* on_pushButtonAllTable_clicked() - изменение пути сохранения всех таблиц
*  расчётов через проводник.
*  Локальная переменная:
*       selDirAllTable - изменённый путь.
*/

void SetupDialog::on_pushButtonAllTable_clicked()
{
    QString selDirAllTable = QFileDialog::getExistingDirectory
            (this, tr("Выберите директорию для сохранения всех таблиц расчётов"));
    if (selDirAllTable.trimmed() != "")
    {                                       // Проверка на выбор новой директории сохранения
        ui->lineEditAllTable->setText(selDirAllTable);
        on_lineEditAllTable_returnPressed();
    }
}

/* on_lineEditSDTable_returnPressed() - изменение пути сохранения таблицы
*  расчётов по ИД вручную.
*/

void SetupDialog::on_lineEditSDTable_returnPressed()
{
    if (QDir(ui->lineEditSDTable->text()).mkpath(".") == false)
    {                                       // Проверка существования директории
        ui->lineEditSDTable->setText(Set->value("changed/dirSDTable").toString());
        QMessageBox::critical
                (this, "Ошибка", "Неверный путь!\n\nИмя файла не должно содержать следующих знаков:\n: * ? \" < > |");
        return;
    }
    ui->lineEditSDTable->setText(QDir(ui->lineEditSDTable->text()).absolutePath());
    Set->setValue("changed/dirSDTable", ui->lineEditSDTable->text());
    QMessageBox::information(this, "Изменение", "Путь к директории\nуспешно изменён!");
}

/* on_pushButtonSDTable_clicked() - изменение пути сохранения таблицы
*  расчётов по ИД через проводник.
*  Локальная переменная:
*       selDirSDTable - изменённый путь.
*/

void SetupDialog::on_pushButtonSDTable_clicked()
{
    QString selDirSDTable = QFileDialog::getExistingDirectory
            (this, tr("Выберите директорию для сохранения таблицы расчётов по ИД"));
    if (selDirSDTable.trimmed() != "")
    {                                       // Проверка на выбор новой директории сохранения
        ui->lineEditSDTable->setText(selDirSDTable);
        on_lineEditSDTable_returnPressed();
    }
}

/* on_lineEditMNITable_returnPressed() - изменение пути сохранения таблицы
*  расчётов по МНИ вручную.
*/

void SetupDialog::on_lineEditMNITable_returnPressed()
{
    if (QDir(ui->lineEditMNITable->text()).mkpath(".") == false)
    {                                       // Проверка существования директории
        ui->lineEditMNITable->setText(Set->value("changed/dirMNITable").toString());
        QMessageBox::critical
                (this, "Ошибка", "Неверный путь!\n\nИмя файла не должно содержать следующих знаков:\n: * ? \" < > |");
        return;
    }
    ui->lineEditMNITable->setText(QDir(ui->lineEditMNITable->text()).absolutePath());
    Set->setValue("changed/dirMNITable", ui->lineEditMNITable->text());
    QMessageBox::information(this, "Изменение", "Путь к директории\nуспешно изменён!");
}

/* on_pushButtonMNITable_clicked() - изменение пути сохранения таблицы
*  расчётов по МНИ через проводник.
*  Локальная переменная:
*       selDirMNITable - изменённый путь.
*/

void SetupDialog::on_pushButtonMNITable_clicked()
{
    QString selDirMNITable = QFileDialog::getExistingDirectory
            (this, tr("Выберите директорию для сохранения таблицы расчётов по МНИ"));
    if (selDirMNITable.trimmed() != "")
    {                                       // Проверка на выбор новой директории сохранения
        ui->lineEditMNITable->setText(selDirMNITable);
        on_lineEditMNITable_returnPressed();
    }
}

/* on_pushButtonDivergTable_clicked() - изменение пути сохранения таблицы
*  отклонений орбит вручную.
*/

void SetupDialog::on_lineEditDivergTable_returnPressed()
{
    if (QDir(ui->lineEditDivergTable->text()).mkpath(".") == false)
    {                                       // Проверка существования директории
        ui->lineEditDivergTable->setText(Set->value("changed/dirDivergTable").toString());
        QMessageBox::critical
                (this, "Ошибка", "Неверный путь!\n\nИмя файла не должно содержать следующих знаков:\n: * ? \" < > |");
        return;
    }
    ui->lineEditDivergTable->setText(QDir(ui->lineEditDivergTable->text()).absolutePath());
    Set->setValue("changed/dirDivergTable", ui->lineEditDivergTable->text());
    QMessageBox::information(this, "Изменение", "Путь к директории\nуспешно изменён!");
}

/* on_pushButtonDivergTable_clicked() - изменение пути сохранения таблицы
*  отклонений орбит через проводник.
*  Локальная переменная:
*       selDirDivergTable - изменённый путь.
*/

void SetupDialog::on_pushButtonDivergTable_clicked()
{
    QString selDirDivergTable = QFileDialog::getExistingDirectory
            (this, tr("Выберите директорию для сохранения таблицы отклонений орбит"));
    if (selDirDivergTable.trimmed() != "")
    {                                       // Проверка на выбор новой директории сохранения
        ui->lineEditDivergTable->setText(selDirDivergTable);
        on_lineEditDivergTable_returnPressed();
    }
}

// on_pushButtonDefault_clicked() - возвращение к настройкам по умолчанию.

void SetupDialog::on_pushButtonDefault_clicked()
{
    if(!QDir(Set->value("default/dir").toString()).exists())
    {                                       // Условие проверки существования
                                            // стандартной директории
        QDir(Set->value("default/dirHomePath").toString()).mkpath
                (Set->value("default/dirName").toString());
    }
    ui->lineEditAll->setText(Set->value("default/dir").toString());
    Set->setValue("changed/dirAll", Set->value("default/dir").toString());
    ui->lineEditSD->setText(Set->value("default/dir").toString());
    Set->setValue("changed/dirSD", Set->value("default/dir").toString());
    ui->lineEditMNI->setText(Set->value("default/dir").toString());
    Set->setValue("changed/dirMNI", Set->value("default/dir").toString());
    ui->lineEditOut->setText(Set->value("default/dir").toString());
    Set->setValue("changed/dirOut", Set->value("default/dir").toString());
    ui->lineEditAllGraph->setText(Set->value("default/dir").toString());
    Set->setValue("changed/dirAllGraph", Set->value("default/dir").toString());
    ui->lineEditAllTable->setText(Set->value("default/dir").toString());
    Set->setValue("changed/dirAllTable", Set->value("default/dir").toString());
    ui->lineEditSDTable->setText(Set->value("default/dir").toString());
    Set->setValue("changed/dirSDTable", Set->value("default/dir").toString());
    ui->lineEditMNITable->setText(Set->value("default/dir").toString());
    Set->setValue("changed/dirMNITable", Set->value("default/dir").toString());
    ui->lineEditDivergTable->setText(Set->value("default/dir").toString());
    Set->setValue("changed/dirDivergTable", Set->value("default/dir").toString());
    QMessageBox::information(this, "Изменение", "Все пути к директориям успешно\nизменены на путь к стандартной директории!\n           (Путь: Downloads/Tmypl Info)");
}
