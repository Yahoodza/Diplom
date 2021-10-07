#include "setupform.h"
#include "ui_setupform.h"

SetupForm::SetupForm(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::SetupForm)
{
    ui->setupUi(this);
    setWindowTitle("Настройки");

    ui->tabWidget->removeTab(1);
    ui->tabWidget->setTabText(0, "Настройки сохранения");

    ui->lineEditAll->setText(Set->value("changed/dirAll").toString());
    ui->lineEditSD->setText(Set->value("changed/dirSD").toString());
    ui->lineEditMNI->setText(Set->value("changed/dirMNI").toString());
    ui->lineEditOut->setText(Set->value("changed/dirOut").toString());
}

SetupForm::~SetupForm()
{
    delete ui;
}

void SetupForm::on_lineEditAll_editingFinished()
{
    QString FilePathStrAll = ui->lineEditAll->text();
    if (FilePathStrAll[FilePathStrAll.length()-1] != "/")
    {
        FilePathStrAll += "/";
        ui->lineEditAll->setText(FilePathStrAll);
    }
    QDir dirAll(FilePathStrAll);

    if (!dirAll.exists())
    {
        dirAll.mkpath(".");        // Создаем директорию, если она не существует
    }
    Set->setValue("changed/dirAll", FilePathStrAll);
}

void SetupForm::on_pushButtonAll_clicked()
{
    QString selDirAll = QFileDialog::getExistingDirectory(this,tr("Выберите директорию для сохранения всего"));
    if (selDirAll.trimmed() != "")
    {
        ui->lineEditAll->setText(selDirAll);
        on_lineEditAll_editingFinished();
    }
}

void SetupForm::on_lineEditSD_editingFinished()
{
    QString FilePathStrSD = ui->lineEditSD->text();
    if (FilePathStrSD[FilePathStrSD.length()-1] != "/")
    {
        FilePathStrSD += "/";
        ui->lineEditSD->setText(FilePathStrSD);
    }
    QDir dirSD(FilePathStrSD);

    if (!dirSD.exists())
    {
        dirSD.mkpath(".");        // Создаем директорию, если она не существует
    }
    Set->setValue("changed/dirSD", FilePathStrSD);
}

void SetupForm::on_pushButtonSD_clicked()
{
    QString selDirSD = QFileDialog::getExistingDirectory(this,tr("Выберите директорию для сохранения ИД"));
    if (selDirSD.trimmed() != "")
    {
        ui->lineEditSD->setText(selDirSD);
        on_lineEditSD_editingFinished();
    }
}

void SetupForm::on_lineEditMNI_editingFinished()
{
    QString FilePathStrMNI = ui->lineEditMNI->text();
    if (FilePathStrMNI[FilePathStrMNI.length()-1] != "/")
    {
        FilePathStrMNI += "/";
        ui->lineEditMNI->setText(FilePathStrMNI);
    }
    QDir dirMNI(FilePathStrMNI);

    if (!dirMNI.exists())
    {
        dirMNI.mkpath(".");        // Создаем директорию, если она не существует
    }
    Set->setValue("changed/dirMNI", FilePathStrMNI);
}

void SetupForm::on_pushButtonMNI_clicked()
{
    QString selDirMNI = QFileDialog::getExistingDirectory(this,tr("Выберите директорию для сохранения ФМНИ"));
    if (selDirMNI.trimmed() != "")
    {
        ui->lineEditMNI->setText(selDirMNI);
        on_lineEditMNI_editingFinished();
    }
}

void SetupForm::on_lineEditOut_editingFinished()
{
    QString FilePathStrOut = ui->lineEditOut->text();
    if (FilePathStrOut[FilePathStrOut.length()-1] != "/")
    {
        FilePathStrOut += "/";
        ui->lineEditOut->setText(FilePathStrOut);
    }
    QDir dirOut(FilePathStrOut);

    if (!dirOut.exists())
    {
        dirOut.mkpath(".");        // Создаем директорию, если она не существует
    }
    Set->setValue("changed/dirOut", FilePathStrOut);
}

void SetupForm::on_pushButtonOut_clicked()
{
    QString selDirOut = QFileDialog::getExistingDirectory(this, tr("Выберите директорию для сохранения выходного набора"));
    if (selDirOut.trimmed() != "")
    {
        ui->lineEditOut->setText(selDirOut);
        on_lineEditOut_editingFinished();
    }
}

void SetupForm::on_pushButtonDefault_clicked()
{
    if(!QDir(Set->value("default/dir").toString()).exists())
    {
        QDir(Set->value("default/dirHomePath").toString()).mkpath(Set->value("default/dirName").toString());
    }
    ui->lineEditAll->setText(Set->value("default/dir").toString());
    Set->setValue("changed/dirAll", Set->value("default/dir").toString());
    ui->lineEditSD->setText(Set->value("default/dir").toString());
    Set->setValue("changed/dirSD", Set->value("default/dir").toString());
    ui->lineEditMNI->setText(Set->value("default/dir").toString());
    Set->setValue("changed/dirMNI", Set->value("default/dir").toString());
    ui->lineEditOut->setText(Set->value("default/dir").toString());
    Set->setValue("changed/dirOut", Set->value("default/dir").toString());
}
