// MainWindow - модуль для взаимодействия с главным окном программы

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDate>
#include <QStyleFactory>
#include <QDebug>

// closeEvent(*) - событие, возникающее при закрытии формы.

void MainWindow::closeEvent(QCloseEvent * )
{
    if (GraphWindow != NULL)
    {
        GraphWindow->GraphDialog::~GraphDialog();
        GraphWindow = NULL;
    }

    if (SetupWindow != NULL)
    {
        SetupWindow->SetupDialog::~SetupDialog();
        SetupWindow = NULL;
    }
}

/* MainWindow() – конструктор класса.
*  Формальный параметр:
*       parent - объект для отображения интерфейса.
*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Информация из файла tmypl");

    SetupWindow = NULL;
    GraphWindow = NULL;

    if(SetSave->value("changed/w_or_d").toString() == "w")
    {                                       // Условие проверки на выбранную
        on_actionWhite_triggered();         // тему программы
    }
    else
    {
        if(SetSave->value("changed/w_or_d").toString() == "d")
        {
            on_actionDark_triggered();
        }
    }

    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->actionReturn->setDisabled(1);
    ui->actionSD->setDisabled(1);
    ui->actionFMNI->setDisabled(1);
    ui->actionSaveAll->setDisabled(1);
    ui->actionSaveAll_AS->setDisabled(1);
    ui->actionSaveSD->setDisabled(1);
    ui->actionSaveSD_AS->setDisabled(1);
    ui->actionSaveFMNI->setDisabled(1);
    ui->actionSaveFMNI_AS->setDisabled(1);
    ui->comboBox->setDisabled(1);
    ui->pushButtonSaveOut->setDisabled(1);
    ui->pushButtonOpenFileOut->setDisabled(1);
    ui->pushButtonViewOut->setDisabled(1);
    ui->actionGraph->setDisabled(1);
}

// ~MainWindow() – деструктор класса.

MainWindow::~MainWindow()
{
    delete ui;
}

/* on_actionOpen_triggered() – выбор файла tmypl.
*  Локальные переменные:
*       PathF - путь к выбранному файлу tmypl;
*       Seans - переменная для проверки на наличие сеансов в файле;
*       MGend - переменная для проверки на наличие МГ в конце файла;
*       fileCheck - файл, который необходимо проверить на принадлежность
*       к виду tmypl;
*       file – выбранный файл;
*       i – счётчик для цикла;
*       j – счётчик для цикла;
*       k – счётчик для осей коэффициента аппроксимирующего полинома;
*       MndiMnMG - массив для записи данных из метода deleteMG();
*       s - переменная для записи первого бита коэффициента;
*       SesSize - размер сеанса ИИ в байтах;
*       pos - счётчик цикла для поиска ФМНИ;
*       KolMNIses - массив для записи структуры ФМНИ;
*       KS - расчёт контрольной суммы формуляра;
*       BufferII - массив данных ИИ;
*       ReversByte - переменная для переворота информации побайтово;
*       Size - размер ИИ в байтах.
*/

void MainWindow::on_actionOpen_triggered()
{
    ui->textEdit->clear();
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->comboBox->clear();
;
    QString PathF = QFileDialog::getOpenFileName
            (this, "Открыть файл tmyp", QDir::homePath() + "/Desktop/", "*tmyp*");

    QFile file(PathF);

    if (!file.open(QIODevice::ReadOnly))    // Условие проверки открытия и
    {                                       // существования файла
        ui->actionReturn->setDisabled(1);
        ui->actionSD->setDisabled(1);
        ui->actionFMNI->setDisabled(1);
        ui->actionSaveAll->setDisabled(1);
        ui->actionSaveAll_AS->setDisabled(1);
        ui->actionSaveSD->setDisabled(1);
        ui->actionSaveSD_AS->setDisabled(1);
        ui->actionSaveFMNI->setDisabled(1);
        ui->actionSaveFMNI_AS->setDisabled(1);
        ui->comboBox->setDisabled(1);
        ui->pushButtonSaveOut->setDisabled(1);
        ui->pushButtonOpenFileOut->setDisabled(1);
        ui->pushButtonViewOut->setDisabled(1);
        ui->actionGraph->setDisabled(1);

        QMessageBox::critical
                (this, "Ошибка", "Файл не был открыт или его не существует");
    }
    else
    {
        if (QFileInfo(PathF).suffix() != "")
        {                                   // Исключение файлов с расширениями
            QMessageBox::critical
                    (this, "Ошибка", "Открыт некорректный файл!\n\nНеобходимо открыть файл вида tmypl");
            return;
        }

        short Seans;
        unsigned char *MGend = new unsigned char[8];
        FILE *fileCheck = fopen(PathF.toLocal8Bit(), "r+b");

        fseek(fileCheck, 4, 0);
        fread(&Seans, sizeof(short), 1, fileCheck);

        fseek(fileCheck, 0, 2);
        fseek(fileCheck, ftell(fileCheck) - 8, 0);
        fread(MGend, 8, 1, fileCheck);
        fclose(fileCheck);

        if((Seans < 1 || Seans > 100) && (MGend[0] != 0xF0) &&
            (MGend[1] != 0x0F) &&           // Проверка на открытие
            (MGend[2] != 0xF0) &&           // файла вида tmypl
            (MGend[3] != 0x0F))
        {
            QMessageBox::critical
                    (this, "Ошибка", "Открыт некорректный файл!\n\nНеобходимо открыть файл вида tmypl");
            return;
        }

        ProgressDialog = new QProgressDialog
                ("Распаковка формуляров...", "&Cancel", 0, 100);
        ProgressDialog->setModal(true);     // Диалоговое окно загрузки

        ProgressDialog->setMinimumDuration(0);
                                            // Диалоговое окно будет показано без задержек
        ProgressDialog->setWindowTitle("Пожалуйста подождите");
        ProgressDialog->setWindowIcon(QIcon(":/image/res/menu/Settings.png"));

        Filename = QFileInfo(file).fileName();
        FileSize = QFileInfo(file).size();
        int i, j, k;

        file.open(QIODevice::ReadOnly);
        QDataStream stream(&file);          // Считывание информации из файла

        stream.readRawData((char *)DataLoc, sizeof(SMID));
        stream.readRawData((char *)DataRem, sizeof(ID0));

        textSD = QString("Комплект ИД в " + Filename +
                         "\n\n---Справочная информация о расположении данных в комплекте ИД---\n" +
                         "\nДлина в байтах комплекта ИД: " + QString::number(DataLoc->length) +
                         "\nПланируемое количество сеансов исходной информации в съеме: " + QString::number(DataLoc->n) +
                         "\nЧисло массивов исходных данных для совместной обработки с КАПС 'О': " + QString::number(DataLoc->nf) +
                         "\n\nЧисло массивов разовых команд: " + QString::number(DataLoc->nrk) +
                         "\nCмещение от начала комплекта ИД для совместной обработки с КАПС 'O': " + QString::number(DataLoc->cmidf) +
                         "\nСмещение от начала комплекта первого пакета разовых команд: " + QString::number(DataLoc->cmmk) +
                         "\nCмещение от начала комплекта информационного массива изделия 14В18: " + QString::number(DataLoc->cmminf) +
                         "\nCмещение от начала комплекта массива справок о положении в комплекте ИД по сеансам включений: " + QString::number(DataLoc->cmspridl) +
                         "\n\n\n---Исходные данные по витку съёма---\n" +
                         "\nНомер объекта: " + QString::number(DataRem->NKA) +
                         "\nНомер витка съёма: " + QString::number(DataRem->NBC) +
                         "\nНомер программы: " + QString::number(DataRem->Nprg) +
                         "\nНомер учетный комплекта ИД: " + QString::number(DataRem->YNid) +
                         "\nГод начала съёма: " + QString::number(DataRem->Year) +
                         "\nМесяц начала съёма: " + QString::number(DataRem->Month) +
                         "\nЧисло начала съёма: " + QString::number(DataRem->Day) +
                         "\nМосковское время начала съёма [сек]: " + QString::number(DataRem->TC) +
                         "\nДлительность режима ВИ [сек]: " + QString::number(DataRem->tvi) +
                         "\nНомер пункта: " + QString::number(DataRem->Nppi) +
                         "\nНомер СР, через который передается информация съема: " + QString::number(DataRem->NSR_L) +
                         "\nКоличество сеансов информации в съёме: " + QString::number(DataRem->ks) +
                         "\n\nПозиционная информация по сеансам включения изделия 14В18:");

        file.seek(66 + 282 + 34 * (DataLoc->nf) + 14 * (DataRem->nrk) + 8200);
        CMarr.clear();

        for (i = 0; i < DataLoc->n; i++)    // Цикл выбора информацию о расположении сеансов ИД
        {
            textSD.append("\n" + QString::number(i+1) +" cеанс : " + QString("%1").arg(DataRem->rr[i], 16, 2, QChar('0')));
            stream.readRawData((char *)ArrRef, sizeof(MSI));
            CMarr.append(*ArrRef);
        }

        textSD.append("\n\nКод типа НУ: " + QString::number(DataRem->RKO_tip) +
                      "\nНомер витка НУ, по которым рассчитаны ИД: " + QString::number(DataRem->RKO) +
                      "\nНомер витка закладки программы: " + QString::number(DataRem->Nvz) +
                      "\nЧисло комплектов массивов разовых команд: " + QString::number(DataRem->nrk) +
                      "\nЧисло комплектов ИД для совместной обработки КТС 'О': " + QString::number(DataRem->nf));

        ID1ses.clear();
        for (i = 0; i < DataLoc->n; i++)    // Цикл выбора информацию по сеансам включений
        {
            file.seek(CMarr[i].CM);
            stream.readRawData((char *)Session, sizeof(ID1));
            ID1ses.append(*Session);

            textSD.append("\n\n\n~~~Исходные данные по сеансу включения №" + QString::number(i+1) + "~~~" +
                          "\nНомер режима работы изделия 14В18 в сеансе: " + QString::number(ID1ses[i].rrs) +
                          "\nПризнак потребителя: " + QString::number(ID1ses[i].ps) +
                          "\nГод начала сеанса: " + QString::number(ID1ses[i].Year) +
                          "\nМесяц начала сеанса: " + QString::number(ID1ses[i].Month) +
                          "\nДень начала сеанса: " + QString::number(ID1ses[i].Day) +
                          "\nЧисло суток в пределах текущего четырехлетия на момент начала сеанса: " + QString::number(ID1ses[i].sutki) +
                          "\nЧисло четырехлетий (с 1-го января 1996г.) на момент начала сеанса: " + QString::number(ID1ses[i].nlet4) +
                          "\nЧисло часов на момент начала сеанса: " + QString::number(ID1ses[i].chas) +
                          "\nЧисло минут на момент начала сеанса: " + QString::number(ID1ses[i].min) +
                          "\nЧисло секунд на момент начала сеанса: " + QString::number(ID1ses[i].sec) +
                          "\nДлительность сеанса [сек]: " + QString::number(ID1ses[i].dt) +
                          "\nКоординаты начала сеанса - широта [рад]: " + QString("%1").arg(ID1ses[i].bn, 0, 'f', 6) +
                          "\nКоординаты начала сеанса - долгота [рад]: " + QString("%1").arg(ID1ses[i].ln, 0, 'f', 6) +
                          "\nКоординаты конца сеанса - широта [рад]: " + QString("%1").arg(ID1ses[i].bk, 0, 'f', 6) +
                          "\nКоординаты конца сеанса - долгота [рад]: " + QString("%1").arg(ID1ses[i].lk, 0, 'f', 6) +
                          "\nДолгота узла [рад]: " + QString("%1").arg(ID1ses[i].uz, 0, 'f', 6) +
                          "\nПеречень номеров районов: " + QString("%1").arg(ID1ses[i].nr, 32, 2, QChar('0')) +
                          "\nТекущий номер витка, на котором производится включение: " + QString::number(ID1ses[i].nv) +
                          "\nНомер суточный витка включения: " + QString::number(ID1ses[i].nd) +
                          "\nВремя прохождения узла [сек]: " + QString("%1").arg(ID1ses[i].tuz, 0, 'f', 6) +
                          "\nВремя сеанса от узла [сек]: " + QString("%1").arg(ID1ses[i].dt0, 0, 'f', 6) +
                          "\nУгол наклонения [рад]: " + QString("%1").arg(ID1ses[i].i, 0, 'f', 6) + "\n");
            k = -1;
            for(j = 0; j <= 24; j++)
            {
                k++;
                if (j <= 7)
                    textSD.append("\n ax" + QString::number(k) + " = " + QString("%1").arg(ID1ses[i].x[j], 0, 'f', 6));
                else
                    if (j <= 15)
                        textSD.append("\n by" + QString::number(k) + " = " + QString("%1").arg(ID1ses[i].x[j], 0, 'f', 6));
                    else
                        if (j <= 23)
                            textSD.append("\n cz" + QString::number(k) + " = " + QString("%1").arg(ID1ses[i].x[j], 0, 'f', 6));
                        else
                            textSD.append("\n t ср (сек) = " + QString("%1").arg(ID1ses[i].x[j], 0, 'f', 6));
                if (k==7)
                {
                    k = -1;
                    textSD.append("\n");
                }
            }

            textSD.append("\n\nМАССИВ РЕЖИМОВ ИЗДЕЛИЯ 14В18 В СЕАНСЕ:");
            textSD.append("\nПредставление массива режимов в виде 16-ричных слов");
            for(j = 0; j < 10; j++)
            {
                if (j == 9)
                    textSD.append("\n СД " + QString::number(j+1) + ": " + QString("%1").arg(ID1ses[i].mr[j], 16, 2, QChar('0')));
                else
                    textSD.append("\n СД  " + QString::number(j+1) + ": " + QString("%1").arg(ID1ses[i].mr[j], 16, 2, QChar('0')));

            }

            textSD.append("\n\nПризнак совместной обработки с КАПС 'О': " + QString::number(ID1ses[i].psop));
            textSD.append("\nЗначение периода обращения: " + QString::number(ID1ses[i].too));

            ui->comboBox->addItem("Сеанс №" + QString::number(i+1));
        }

        ui->textEdit->setText(textSD);

        stream.readRawData((char *)SpravOSprav, sizeof(SMsp_type));

        ui->tableWidget->clear();           // Создание таблицы сеансов
        ui->tableWidget->horizontalHeader()->setVisible(true);
        ui->tableWidget->setRowCount(DataLoc->n);
        ui->tableWidget->setColumnCount(3);
        ui->tableWidget->setHorizontalHeaderLabels
                (QStringList() << "Номер сеанса" << "Адрес начала" << "Адрес конца");

        SesSprav.clear();
        Start.clear();
        End.clear();
        for(i = 0; i < DataLoc->n; i++)     // Цикл определения границ сеансов ИИ
        {
            stream.readRawData((char *)Sprav, sizeof(sSPRAV_type));
            SesSprav.append(*Sprav);

            Start.append(__min(SesSprav[i].adrnMB, SesSprav[i].adrnb));
            End.append(__max(SesSprav[i].adrkMB, SesSprav[i].adrkb));

            TmyplSession(i, Start[i], End[i]);
        }

        BufferSeans.clear();
        MndiSes.clear();
        MnSes.clear();
        QVector<int> MndiMnMG;
        for(i = 0; i < DataLoc->n; i++)     // Цикл редактирования и записи данных из
        {                                   // сеанса ИИ
            MndiMnMG.clear();
            ProgressDialog->setValue(9 * i);
            qApp->processEvents();

            file.seek(Start[i]);
            BufferSeans.append(file.read(End[i] - Start[i]));
            turnOver(i);

            ProgressDialog->setValue(3 + 9 * i);
            qApp->processEvents();

            MndiMnMG = deleteMG(i);
            MndiSes.append(MndiMnMG.at(0));
            MnSes.append(MndiMnMG.at(1));

            quantityFormular(i);

            ProgressDialog->setValue(6 + 9 * i);
            qApp->processEvents();
        }

        char s;
        unsigned long SesSize;
        unsigned int pos;
        QVector<MNI_type> KolMNIses;
        unsigned long KS;
        FMNIses.clear();
        textMNI = ("~~~Формуляры массива навигационной информации~~~");

        for(i = 0; i < DataLoc->n; i++)     // Цикл смены сеанса
        {
            ProgressDialog->setValue(45 + 9 * i);
            qApp->processEvents();

            SesSize = BufferSeans[i].size();
            textMNI.append("\n\n\nФМНИ из Сеанса " + QString::number(i+1) + ":");
            KolMNIses.clear();
            j = 0;

            for(pos = 0; pos < SesSize; pos += 8)
            {                               // Вложенный цикл для записи ФМНИ
                memset(FMNI, 0, sizeof (MNI_type));
                if((((unsigned char)BufferSeans[i][pos] & 0xF8) == 0x78) &&
                  (((((unsigned char)BufferSeans[i][pos+1] & 0xE0) >> 5) == 0x2)) &&
                  ((((unsigned char)BufferSeans[i][pos] & 0x04) >> 2) == 0x0) &&
                  ((((unsigned char)BufferSeans[i][pos] & 0x02) >> 1) == 0x0))
                {                           // Условие выбора формуляров МНИ
                    FMNI->TypeMes = ((BufferSeans[i][pos+1] & 0xE0)>>5);

                    FMNI->qGroup = (BufferSeans[i][pos+1] & 0x1F);

                    FMNI->NumMas = ((BufferSeans[i][pos+2] & 0xF8)>>3);

                    FMNI->cMV4l = ((BufferSeans[i][pos+4] & 0xF0)>>4);

                    FMNI->cMVday = ((BufferSeans[i][pos+5] & 0xFE)>>1) + ((BufferSeans[i][pos+4] & 0x0F)<<7);

                    FMNI->cMVhour = ((BufferSeans[i][pos+6] & 0xF0)>>4) + ((BufferSeans[i][pos+5] & 0x01)<<4);

                    FMNI->cMVmin = ((BufferSeans[i][pos+7] & 0xC0)>>6) + ((BufferSeans[i][pos+6] & 0x0F)<<2);

                    FMNI->cMVsec = (BufferSeans[i][pos+7] & 0x3F);

                    KS = 0;
                    for (unsigned short k = 2; k < (8 * 8 - 2); k += 2)
                    {
                        if (!(k % 8))
                                continue;
                        KS = KS + (BufferSeans[i][pos+k+1]&0xFF) + ((BufferSeans[i][pos+k]&0xFF) << 8);
                    }

                    while ((KS&0xFFFF0000) != 0)
                            KS = ((KS&0xFFFF0000) >> 16) + (KS&0x0000FFFF);

                    FMNI->KS = KS;

                    FMNI->KSMNI = (BufferSeans[i][pos+63] & 0xFF) + ((BufferSeans[i][pos+62] & 0xFF)<<8);

                    FMNI->coefA0X = (BufferSeans[i][pos+13] & 0xFF) + ((BufferSeans[i][pos+12] & 0xFF)<<8) + ((BufferSeans[i][pos+11] & 0xFF)<<16) + ((BufferSeans[i][pos+10] & 0x7F)<<24);
                    s = ((BufferSeans[i][pos+10] & 0x80) >> 7);
                    if (s == 1) FMNI->coefA0X -= 2147483648; //FMNI->coefA0X *= -1;

                    FMNI->coefA0Y = (BufferSeans[i][pos+19] & 0xFF) + ((BufferSeans[i][pos+18] & 0xFF)<<8) + ((BufferSeans[i][pos+15] & 0xFF)<<16) + ((BufferSeans[i][pos+14] & 0x7F)<<24);
                    s = ((BufferSeans[i][pos+14] & 0x80) >> 7);
                    if (s == 1) FMNI->coefA0Y -= 2147483648; //FMNI->coefA0Y *= -1;

                    FMNI->coefA0Z = (BufferSeans[i][pos+23] & 0xFF) + ((BufferSeans[i][pos+22] & 0xFF)<<8) + ((BufferSeans[i][pos+21] & 0xFF)<<16) + ((BufferSeans[i][pos+20] & 0x7F)<<24);
                    s = ((BufferSeans[i][pos+20] & 0x80) >> 7);
                    if (s == 1) FMNI->coefA0Z -= 2147483648; //FMNI->coefA0Z *= -1;

                    FMNI->coefA1X = (BufferSeans[i][pos+29] & 0xFF) + ((BufferSeans[i][pos+28] & 0xFF)<<8) + ((BufferSeans[i][pos+27] & 0xFF)<<16) + ((BufferSeans[i][pos+26] & 0x7F)<<24);
                    s = ((BufferSeans[i][pos+26] & 0x80) >> 7);
                    if (s == 1) FMNI->coefA1X -= 2147483648; //FMNI->coefA1X *= -1;

                    FMNI->coefA1Y = (BufferSeans[i][pos+35] & 0xFF) + ((BufferSeans[i][pos+34] & 0xFF)<<8) + ((BufferSeans[i][pos+31] & 0xFF)<<16) + ((BufferSeans[i][pos+30] & 0x7F)<<24);
                    s = ((BufferSeans[i][pos+30] & 0x80) >> 7);
                    if (s == 1) FMNI->coefA1Y -= 2147483648; //FMNI->coefA1Y *= -1;

                    FMNI->coefA1Z = (BufferSeans[i][pos+39] & 0xFF) + ((BufferSeans[i][pos+38] & 0xFF)<<8) + ((BufferSeans[i][pos+37] & 0xFF)<<16) + ((BufferSeans[i][pos+36] & 0x7F)<<24);
                    s = ((BufferSeans[i][pos+36] & 0x80) >> 7);
                    if (s == 1) FMNI->coefA1Z -= 2147483648; //FMNI->coefA1Z *= -1;

                    FMNI->coefA2X = (BufferSeans[i][pos+45] & 0xFF) + ((BufferSeans[i][pos+44] & 0xFF)<<8) + ((BufferSeans[i][pos+43] & 0xFF)<<16) + ((BufferSeans[i][pos+42] & 0x7F)<<24);
                    s = ((BufferSeans[i][pos+42] & 0x80) >> 7);
                    if (s == 1) FMNI->coefA2X -= 2147483648; //FMNI->coefA2X *= -1;

                    FMNI->coefA2Y = (BufferSeans[i][pos+51] & 0xFF) + ((BufferSeans[i][pos+50] & 0xFF)<<8) + ((BufferSeans[i][pos+47] & 0xFF)<<16) + ((BufferSeans[i][pos+46] & 0x7F)<<24);
                    s = ((BufferSeans[i][pos+46] & 0x80) >> 7);
                    if (s == 1) FMNI->coefA2Y -= 2147483648; //FMNI->coefA2Y *= -1;

                    FMNI->coefA2Z = (BufferSeans[i][pos+55] & 0xFF) + ((BufferSeans[i][pos+54] & 0xFF)<<8) + ((BufferSeans[i][pos+53] & 0xFF)<<16) + ((BufferSeans[i][pos+52] & 0x7F)<<24);
                    s = ((BufferSeans[i][pos+52] & 0x80) >> 7);
                    if (s == 1) FMNI->coefA2Z -= 2147483648; //FMNI->coefA2Z *= -1;

                    j++;
                    textMNI.append("\n\nФМНИ №" + QString::number(j) + ":" +
                                  "\nТип сообщения: " + QString::number(FMNI->TypeMes) +
                                  "\nКоличество групп: " + QString::number(FMNI->qGroup) +
                                  "\nНомер массива МНИ: " + QString::number(FMNI->NumMas) +
                                  "\nТекущее четырёхлетие: " + QString::number(FMNI->cMV4l) +
                                  "\nТекущее число суток: " + QString::number(FMNI->cMVday) +
                                  "\nТекущее число часов: " + QString::number(FMNI->cMVhour) +
                                  "\nТекущее число минут: " + QString::number(FMNI->cMVmin) +
                                  "\nТекущее число секунд: " + QString::number(FMNI->cMVsec) +
                                  "\nКоэффициент A0x: " + QString("%1").arg(FMNI->coefA0X, -15, 10, QChar(' ')) +
                                  "Коэффициент A0y: " + QString("%1").arg(FMNI->coefA0Y, -15, 10, QChar(' ')) +
                                  "Коэффициент A0z: " + QString("%1").arg(FMNI->coefA0Z, -15, 10, QChar(' ')) +
                                  "\nКоэффициент A1x: " + QString("%1").arg(FMNI->coefA1X, -15, 10, QChar(' ')) +
                                  "Коэффициент A1y: " + QString("%1").arg(FMNI->coefA1Y, -15, 10, QChar(' ')) +
                                  "Коэффициент A1z: " + QString("%1").arg(FMNI->coefA1Z, -15, 10, QChar(' ')) +
                                  "\nКоэффициент A2x: " + QString("%1").arg(FMNI->coefA2X, -15, 10, QChar(' ')) +
                                  "Коэффициент A2y: " + QString("%1").arg(FMNI->coefA2Y, -15, 10, QChar(' ')) +
                                  "Коэффициент A2z: " + QString("%1").arg(FMNI->coefA2Z, -15, 10, QChar(' ')) +
                                  "\nКонтрольная сумма формуляра МНИ: " + QString::number(FMNI->KSMNI) +
                                  "\nРассчитанная контрольная сумма формуляра МНИ: " + QString::number(FMNI->KS));

                    KolMNIses.append(*FMNI);
                }
            }
            ProgressDialog->setValue(48 + 9 * i);
            qApp->processEvents();

            FMNIses.append(KolMNIses);

            ProgressDialog->setValue(51 + 9 * i);
            qApp->processEvents();
        }

        ProgressDialog->setValue(90);
        qApp->processEvents();

        ui->actionReturn->setDisabled(0);
        ui->actionSD->setDisabled(0);
        ui->actionFMNI->setDisabled(0);
        ui->actionSaveAll->setDisabled(0);
        ui->actionSaveAll_AS->setDisabled(0);
        ui->actionSaveSD->setDisabled(0);
        ui->actionSaveSD_AS->setDisabled(0);
        ui->actionSaveFMNI->setDisabled(0);
        ui->actionSaveFMNI_AS->setDisabled(0);
        ui->comboBox->setDisabled(0);
        ui->pushButtonSaveOut->setDisabled(0);
        ui->pushButtonOpenFileOut->setDisabled(0);
        ui->pushButtonViewOut->setDisabled(0);
        ui->actionGraph->setDisabled(0);

        QByteArray BufferII;
        file.seek(DataLoc->length + 5000);
        BufferII.append(file.read(FileSize - DataLoc->length - 5000));

        ProgressDialog->setValue(99);
        qApp->processEvents();

        QProgressDialog *ProgressDialog1 = new QProgressDialog
                ("Подсчёт Мнди и Мн...", "&Cancel", 0, 100);
        ProgressDialog1->setModal(true);    // Диалоговое окно загрузки

        ProgressDialog1->setMinimumDuration(0);
                                            // Диалоговое окно будет показано без задержек
        ProgressDialog1->setWindowTitle("Пожалуйста подождите");
        ProgressDialog1->setWindowIcon(QIcon(":/image/res/menu/Settings.png"));

        Mndi = 0;
        Mn = 0;
        for (i = 0; i != BufferII.count() - 8; i += 8)
        {                                   // Цикл подсчёта Мнди и Мн в ИИ без учёта МГ
            ProgressDialog1->setValue(100 * i/ BufferII.count());
            qApp->processEvents();

            if(((unsigned char)BufferII[i] == 0xF0) &&
               ((unsigned char)BufferII[i+1] == 0x0F) &&
               ((unsigned char)BufferII[i+2] == 0xF0) &&
               ((unsigned char)BufferII[i+3] == 0x0F))
            {
                BufferII.remove(i, 8);
            }

            if(((((unsigned char)BufferII[i] & 0x02) >> 2) == 0x1))
                Mndi++;

            if(((((unsigned char)BufferII[i] & 0x04) >> 1) == 0x1))
                Mn++;
        }

        ProgressDialog1->setValue(100);

        if (ProgressDialog != NULL)         // Условие завершения загрузки
        {
            delete ProgressDialog;
            ProgressDialog = NULL;
        }

        if (ProgressDialog1 != NULL)        // Условие завершения загрузки
        {
            delete ProgressDialog1;
            ProgressDialog1 = NULL;
        }

    }
}

// on_actionFMNI_triggered() - просмотр ИД.

void MainWindow::on_actionSD_triggered()
{
    ui->textEdit->setEnabled(1);
    ui->textEdit->setText(textSD);
}

// on_actionSD_triggered() - просмотр ФМНИ.

void MainWindow::on_actionFMNI_triggered()
{
    ui->textEdit->setEnabled(1);
    ui->textEdit->setText(textMNI);
}

/* on_actionSaveAll_triggered() - сохранение ИД и ФМНИ в заданную директорию.
*  Локальные переменные:
*       Path – путь к файлу в заданной директории;
*       file1 – файл для записи ИД;
*       file2 – файл для записи ФМНИ.
*/

void MainWindow::on_actionSaveAll_triggered()
{
    if(!QDir(SetSave->value("changed/dirAll").toString()).exists())
    {                                       // Условие проверки существования директории
        QMessageBox::critical
                (this, "Ошибка", "Файлы не сохранены, проверьте путь сохранения!");
    }
    else
    {
        QString Path = QString("%1/Печать ИД " + Filename + ".txt").arg(SetSave->value("changed/dirAll").toString());
        QFile file1(Path);
        if (file1.open(QFile::WriteOnly | QFile::Text))
        {                                   // Условие записи информации в файл
            file1.write(textSD.toUtf8());
        }
        file1.close();

        Path = QString("%1/Печать ФМНИ " + Filename + ".txt").arg(SetSave->value("changed/dirAll").toString());
        QFile file2(Path);
        if (file2.open(QFile::WriteOnly | QFile::Text))
        {
            file2.write(textMNI.toUtf8());
        }
        file2.close();

        QMessageBox::information(this, "Сохранение", "Файлы печати ИД и печати МНИ успешно сохранены\n\nПуть: " + SetSave->value("changed/dirAll").toString());
    }
}

/* on_actionSaveAll_AS_triggered() - сохранение ИД и ФМНИ с возможностью
*  выбора директории.
*  Локальные переменные:
*       dir – выбор директории;
*       file1 – файл для записи ИД;
*       file2 – файл для записи ФМНИ.
*/

void MainWindow::on_actionSaveAll_AS_triggered()
{
    QString dir = QFileDialog::getSaveFileName
            (this, tr("Выберите директорию сохранения файла печати ИД"), "", "*.txt");
    QFile file1(dir);
    if (file1.open(QFile::WriteOnly | QFile::Text))
    {
        file1.write(textSD.toUtf8());
    }
    file1.close();

    dir = QFileDialog::getSaveFileName
            (this, tr("Выберите директорию сохранения файла печати ФМНИ"), "", "*.txt");
    QFile file2(dir);
    if (file2.open(QFile::WriteOnly | QFile::Text))
    {
        file2.write(textMNI.toUtf8());
    }
    file2.close();
}

/* on_actionSaveSD_triggered() - сохранение ИД в заданную директорию.
*  Локальные переменные:
*       Path – путь к файлу в заданной директории;
*       file – файл для записи ИД.
*/

void MainWindow::on_actionSaveSD_triggered()
{
    if(!QDir(SetSave->value("changed/dirSD").toString()).exists())
    {                                       // Условие проверки существования директории
        QMessageBox::critical
                (this, "Ошибка", "Файлы не сохранены, проверьте путь сохранения!");
    }
    else
    {
        QString Path = QString("%1/Печать ИД " + Filename + ".txt").arg(SetSave->value("changed/dirSD").toString());
        QFile file(Path);
        if (file.open(QFile::WriteOnly | QFile::Text))
        {
            file.write(textSD.toUtf8());
        }
        file.close();

        QMessageBox::information(this, "Сохранение", "Файл печати ИД успешно сохранён\n\nПуть: " + SetSave->value("changed/dirSD").toString());
    }
}

/* on_actionSaveSD_AS_triggered() - сохранение ИД с возможностью
*  выбора директории.
*  Локальные переменные:
*       dir – выбор директории;
*       file – файл для записи ИД.
*/

void MainWindow::on_actionSaveSD_AS_triggered()
{
    QString dir = QFileDialog::getSaveFileName
            (this, tr("Выберите директорию сохранения файла печати ИД"), "", "*.txt");
    QFile file(dir);
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        file.write(textSD.toUtf8());
    }
    file.close();
}

/* on_actionSaveFMNI_triggered() - сохранение ФМНИ в заданную директорию.
*  Локальные переменные:
*       Path – путь к файлу в заданной директории;
*       file – файл для записи ФМНИ.
*/

void MainWindow::on_actionSaveFMNI_triggered()
{
    if(!QDir(SetSave->value("changed/dirMNI").toString()).exists())
    {                                       // Условие проверки существования директории
        QMessageBox::critical
                (this, "Ошибка", "Файлы не сохранены, проверьте путь сохранения!");
    }
    else
    {
        QString Path = QString("%1/Печать ФМНИ " + Filename + ".txt").arg(SetSave->value("changed/dirMNI").toString());
        QFile file(Path);
        if (file.open(QFile::WriteOnly | QFile::Text))
        {
            file.write(textMNI.toUtf8());
        }
        file.close();

        QMessageBox::information(this, "Сохранение", "Файл печати ФМНИ успешно сохранён\n\nПуть: " + SetSave->value("changed/dirMNI").toString());
    }
}

/* on_actionSaveFMNI_AS_triggered() - сохранение ФМНИ с возможностью
*  выбора директории.
*  Локальные переменные:
*       dir – выбор директории;
*       file – файл для записи ФМНИ.
*/

void MainWindow::on_actionSaveFMNI_AS_triggered()
{
    QString dir = QFileDialog::getSaveFileName
            (this, tr("Выберите директорию сохранения файла печати ФМНИ"), "", "*.txt");
    QFile file(dir);
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        file.write(textMNI.toUtf8());
    }
    file.close();
}

/* TmyplSession() - создание таблицы сеансов tmypl.
*  Формальные параметры:
*       i - сеанс ИИ;
*       st - адрес начала сеанса;
*       en - адрес конца сеанса.
*  Локальные переменные:
*       cell1 – элемент tableWidget для записи номеров сеанса;
*       cell2 – элемент tableWidget для записи адреса начала сеанса;
*       cell3 – элемент tableWidget для записи адреса конца сеанса.
*/

void MainWindow::TmyplSession(unsigned int i, unsigned long st, unsigned long en)
{
    QTableWidgetItem *cell1 = new QTableWidgetItem("Сеанс " + QString::number(i+1));
    QTableWidgetItem *cell2 = new QTableWidgetItem(QString::number(st));
    QTableWidgetItem *cell3 = new QTableWidgetItem(QString::number(en));

    ui->tableWidget->setItem(i, 0, cell1);
    ui->tableWidget->setItem(i, 1, cell2);
    ui->tableWidget->setItem(i, 2, cell3);
}

/* on_tableWidget_itemDoubleClicked() - просмотр ФМНИ выбранного
*  сеанса в виде таблицы.
*  Формальный параметр:
*       item - элемент tableWidget.
*  Локальные переменные:
*       i – выбранный сеанс ИИ;
*       j – счётчик для цикла;
*       k – счётчик для записи данных в таблицу;
*       cell1 - элемент tableWidget для записи параметров ФМНИ.
*/

void MainWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    int i = item->row();
    int j = 0;
    int k = 0;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(quantityFormular(i)*4);
    ui->tableWidget->setColumnCount(12);
    ui->tableWidget->setHorizontalHeaderLabels
            (QStringList() << "Порядковый номер МНИ" << "Формуляр" << "П"
             << "А" << "Р" << "А" << "М" << "Е" << "Т" << "Р" << "Ы" << "!");

    for(j = 0; j < (int)quantityFormular(i); j++)
    {                                       // Цикл записи МНИ в таблицу
        QTableWidgetItem *cell1 = new QTableWidgetItem(QString::number(j+1));
        ui->tableWidget->setItem(k, 0, cell1);
        cell1->setBackground(Qt::darkGray);

        cell1 = new QTableWidgetItem("МНИ");
        cell1->setBackground(QColor("#02b906"));
        ui->tableWidget->setItem(k, 1, cell1);

        cell1 = new QTableWidgetItem("ТС");
        ui->tableWidget->setItem(k, 2, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].TypeMes));
        ui->tableWidget->setItem(k+1, 2, cell1);

        cell1 = new QTableWidgetItem("КГ");
        ui->tableWidget->setItem(k, 3, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].qGroup));
        ui->tableWidget->setItem(k+1, 3, cell1);

        cell1 = new QTableWidgetItem("Номер МНИ");
        ui->tableWidget->setItem(k, 4, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].NumMas));
        ui->tableWidget->setItem(k+1, 4, cell1);

        cell1 = new QTableWidgetItem("4-летие");
        ui->tableWidget->setItem(k, 5, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].cMV4l));
        ui->tableWidget->setItem(k+1, 5, cell1);

        cell1 = new QTableWidgetItem("СУТКИ");
        ui->tableWidget->setItem(k, 6, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].cMVday));
        ui->tableWidget->setItem(k+1, 6, cell1);

        cell1 = new QTableWidgetItem("ЧАСЫ");
        ui->tableWidget->setItem(k, 7, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].cMVhour));
        ui->tableWidget->setItem(k+1, 7, cell1);

        cell1 = new QTableWidgetItem("МИНУТЫ");
        ui->tableWidget->setItem(k, 8, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].cMVmin));
        ui->tableWidget->setItem(k+1, 8, cell1);

        cell1 = new QTableWidgetItem("СЕКУНДЫ");
        ui->tableWidget->setItem(k, 9, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].cMVsec));
        ui->tableWidget->setItem(k+1, 9, cell1);

        cell1 = new QTableWidgetItem("КС");
        ui->tableWidget->setItem(k, 10, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].KSMNI));
        ui->tableWidget->setItem(k+1, 10, cell1);

        cell1 = new QTableWidgetItem("КСрасч");
        ui->tableWidget->setItem(k, 11, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].KS));
        ui->tableWidget->setItem(k+1, 11, cell1);

        cell1 = new QTableWidgetItem("A0x");
        ui->tableWidget->setItem(k+2, 2, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].coefA0X));
        ui->tableWidget->setItem(k+3, 2, cell1);

        cell1 = new QTableWidgetItem("A0y");
        ui->tableWidget->setItem(k+2, 3, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].coefA0Y));
        ui->tableWidget->setItem(k+3, 3, cell1);

        cell1 = new QTableWidgetItem("A0z");
        ui->tableWidget->setItem(k+2, 4, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].coefA0Z));
        ui->tableWidget->setItem(k+3, 4, cell1);

        cell1 = new QTableWidgetItem("A1x");
        ui->tableWidget->setItem(k+2, 5, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].coefA1X));
        ui->tableWidget->setItem(k+3, 5, cell1);

        cell1 = new QTableWidgetItem("A1y");
        ui->tableWidget->setItem(k+2, 6, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].coefA1Y));
        ui->tableWidget->setItem(k+3, 6, cell1);

        cell1 = new QTableWidgetItem("A1z");
        ui->tableWidget->setItem(k+2, 7, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].coefA1Z));
        ui->tableWidget->setItem(k+3, 7, cell1);

        cell1 = new QTableWidgetItem("A2x");
        ui->tableWidget->setItem(k+2, 8, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].coefA2X));
        ui->tableWidget->setItem(k+3, 8, cell1);

        cell1 = new QTableWidgetItem("A2y");
        ui->tableWidget->setItem(k+2, 9, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].coefA2Y));
        ui->tableWidget->setItem(k+3, 9, cell1);

        cell1 = new QTableWidgetItem("A2z");
        ui->tableWidget->setItem(k+2, 10, cell1);
        cell1->setBackground(Qt::darkGray);
        cell1 = new QTableWidgetItem(QString::number(FMNIses[i][j].coefA2Z));
        ui->tableWidget->setItem(k+3, 10, cell1);

        k += 4;
    }
}

/* on_actionReturn_triggered() - возврат к таблице выбора сеанса.
*  Локальная переменная:
*       i – счётчик для цикла.
*/

void MainWindow::on_actionReturn_triggered()
{
    int i;
    ui->tableWidget->clear();
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->setRowCount(DataLoc->n);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels
            (QStringList() << "Номер сеанса" << "Адрес начала" << "Адрес конца");

    for(i = 0; i < DataLoc->n; i++)         // Цикл возврата к таблице сеансов
    {
        TmyplSession(i, Start[i], End[i]);
    }
}

/* turnOver() - переворот сеанса ИИ побайтово.
*  Формальный параметр:
*       i - номер сеанса ИИ.
*  Локальные переменные:
*       ReversByte – переменная для переворота информации побайтово;
*       BufferSize – размер сеанса ИИ в байтах;
*       pos – счётчик цикла для переворота сеанса побайтово.
*/

void MainWindow::turnOver(unsigned int i)
{
    unsigned char ReversByte;
    unsigned long BufferSize = BufferSeans[i].size();
    for (unsigned int pos = 0; pos < BufferSize; pos++)
    {                                       // Цикл переворота побайтово
        ReversByte = 0;

        ReversByte |= ((BufferSeans[i][pos]&0x01) << 7);
        ReversByte |= ((BufferSeans[i][pos]&0x02) << 5);
        ReversByte |= ((BufferSeans[i][pos]&0x04) << 3);
        ReversByte |= ((BufferSeans[i][pos]&0x08) << 1);

        ReversByte |= ((BufferSeans[i][pos]&0x10) >> 1);
        ReversByte |= ((BufferSeans[i][pos]&0x20) >> 3);
        ReversByte |= ((BufferSeans[i][pos]&0x40) >> 5);
        ReversByte |= ((BufferSeans[i][pos]&0x80) >> 7);

        BufferSeans[i][pos] = ReversByte;
    }
}

/* deleteMG() - удаление МГ сеанса и подсчёт Мнди и Мн сеанса.
*  Формальный параметр:
*       i - номер сеанса ИИ.
*  Локальные переменные:
*       kolMndiMnMG – массив, хранящий в себе количество МГ,
*       Мнди и Мн сеанса;
*       kolMndi – количество Мнди сеанса;
*       kolMn - количество Мн сеанса;
*       MGQuantity - количество МГ сеанса;
*       pos - счётчик для цикла с шагом в 8 байт.
*/

QVector<int> MainWindow::deleteMG(unsigned int i)
{
    QVector<int> kolMndiMnMG;
    unsigned short kolMndi = 0, kolMn = 0, MGQuantity = 0;
    int pos;

    for(pos = 0; pos < BufferSeans[i].count() - 8; pos += 8)
    {                                       // Цикл удаления МГ и подсчёта Мнди и Мн
        if(((unsigned char)BufferSeans[i][pos] == 0x0F) &&
           ((unsigned char)BufferSeans[i][pos+1] == 0xF0) &&
           ((unsigned char)BufferSeans[i][pos+2] == 0x0F) &&
           ((unsigned char)BufferSeans[i][pos+3] == 0xF0))
        {                                   // Условие поиска МГ
            BufferSeans[i].remove(pos, 8);
            MGQuantity++;
        }

        if(((((unsigned char)BufferSeans[i][pos] & 0x04) >> 2) == 0x1))
            kolMndi++;                      // Условие поиска Мнди

        if(((((unsigned char)BufferSeans[i][pos] & 0x02) >> 1) == 0x1))
            kolMn++;                        // Условие поиска Мн
    }

    kolMndiMnMG.append(kolMndi);
    kolMndiMnMG.append(kolMn);
    kolMndiMnMG.append(MGQuantity);
    return kolMndiMnMG;
}

/* quantityFormular() - расчёт количества ФМНИ в сеансе.
*  Формальный параметр:
*       i - номер сеанса ИИ.
*  Локальные переменные:
*       quantity – количество формуляров МНИ сеанса;
*       BufferSize – размер сеанса ИИ в байтах;
*       pos - счётчик для цикла с шагом в 8 байт.
*/

unsigned long MainWindow::quantityFormular(unsigned int i)
{
    long quantity = 0;
    unsigned long BufferSize = BufferSeans[i].size();

    for(unsigned int pos = 0; pos < BufferSize; pos += 8)
    {                                       // Цикл подсчёта формуляров МНИ
        if((((unsigned char)BufferSeans[i][pos] & 0xF8) == 0x78) &&
          (((((unsigned char)BufferSeans[i][pos+1] & 0xE0) >> 5) == 0x2)) &&
          ((((unsigned char)BufferSeans[i][pos] & 0x04) >> 2) == 0x0) &&
          ((((unsigned char)BufferSeans[i][pos] & 0x02) >> 1) == 0x0))
            quantity++;                     // Условие поиска формуляров МНИ
    }

    return quantity;
}

/* on_pushButtonSaveOut_clicked() - сохранение выходного набора с
*  расширением .bin и .txt в заданную директорию.
*  Локальные переменные:
*       TimeStart – время начала обработки выходного набора;
*       Date – дата обработки выходного набора;
*       j - счётчик для цикла;
*       k - счётчик для осей коэффициента аппроксимирующего полинома;
*       std_string - имя файла информационного массива tmypl;
*       lengthII - размер ИИ в байтах;
*       lengthSes - размер сеанса ИИ в байтах;
*       kolMNIses - количество формуляров МНИ сеанса;
*       Path - путь к директории сохранения выходного набора;
*       filebin - бинарный файл для записи выходного набора;
*       TimeEnd - время конца обработки выходного набора;
*       Outtxt - выходной набор с пояснением каждого параметра;
*       filetxt - текстовый файл для записи выходного набора.
*/

void MainWindow::on_pushButtonSaveOut_clicked()
{
    QTime TimeStart = QTime::currentTime();
    Output->TimeStart = TimeStart.hour() * 3600 + TimeStart.minute() * 60 + TimeStart.second();
    ComboIndex = ui->comboBox->currentIndex();
    QDate Date = QDate::currentDate();
    int j, k;

    std::string std_string = Filename.toStdString();
    unsigned long lengthII = FileSize - DataLoc->length - 5000;
    unsigned long lengthSes = End[ComboIndex] - Start[ComboIndex];
    short kolMNIses = quantityFormular(ComboIndex);

    strcpy(Output->NameMas, std_string.c_str());
    Output->YNsd = DataRem->YNid;           // Запись в структуру
    Output->n = DataLoc->n;                 // выходного набора
    Output->YearStart = Date.year();
    Output->MonthStart = Date.month();
    Output->DayStart = Date.day();
    Output->YearEnd = Date.year();
    Output->MonthEnd = Date.month();
    Output->DayEnd = Date.day();
    Output->SizeTmypl = FileSize;
    Output->lengthSD = DataLoc->length;
    Output->lengthII = lengthII;
    Output->kolIG = qCeil((float)lengthII / 8);
    Output->kolIB = qCeil((float)lengthII / 4096);
    Output->Mndi = Mndi;
    Output->Mn = Mn;
    Output->SesN = ComboIndex + 1;
    Output->YearSes = Date.year();
    Output->MonthSes = Date.month();
    Output->DaySes = Date.day();
    Output->chasSes = TimeStart.hour();
    Output->minSes = TimeStart.minute();
    Output->secSes = TimeStart.second();
    Output->YNsdSes = DataRem->YNid;
    Output->lengthSes = lengthSes;
    Output->kolIGses = qCeil((float)lengthSes / 8);
    Output->kolIBses = qCeil((float)lengthSes / 4096);
    Output->MndiSes = MndiSes[ComboIndex];
    Output->MnSes = MnSes[ComboIndex];
    Output->kolMNIses = kolMNIses;
    Output->offset = (unsigned long)Start[ComboIndex];

    if(!QDir(SetSave->value("changed/dirOut").toString()).exists())
    {                                       // Условие проверки существования директории
        QMessageBox::critical(this, "Ошибка", "Файлы не сохранены, проверьте путь сохранения!");
    }
    else
    {
        QString Path = QString("%1/rezult" + QString::number(ComboIndex + 1) + " " + Output->NameMas + ".bin").arg(SetSave->value("changed/dirOut").toString());

        FILE *filebin = fopen(Path.toLocal8Bit(), "w+b");

        QTime TimeEnd = QTime::currentTime();
        Output->TimeEnd = TimeEnd.hour() * 3600 + TimeEnd.minute() * 60 + TimeEnd.second();
        fwrite(Output, sizeof (OutputSet_type), 1, filebin);
        fwrite(&ID1ses[ComboIndex], sizeof (ID1), 1, filebin);
        for(int i = 0; i < kolMNIses; i++)  // Цикл записи в файл формуляров МНИ
        {
            fwrite(&FMNIses[ComboIndex][i], sizeof (MNI_type), 1, filebin);
        }
        fclose(filebin);

        Path = QString("%1/rezult" + QString::number(ComboIndex + 1) + " " + Output->NameMas + ".txt").arg(SetSave->value("changed/dirOut").toString());

        QString Outtxt = QString("Выходной набор " + QString(Output->NameMas) +
                                 "\n\n\n---Учетная информация исходного массива---\n" +
                                 "\nУчётный номер комплекта исходных данных: " + QString::number(Output->YNsd) +
                                 "\nКоличество сеансов в исходном массиве: " + QString::number(Output->n) +
                                 "\nГод начала обработки: " + QString::number(Output->YearStart) +
                                 "\nМесяц начала обработки: " + QString::number(Output->MonthStart) +
                                 "\nДень начала обработки: " + QString::number(Output->DayStart) +
                                 "\nМосковское время (в секундах) начала обработки: " + QString::number(Output->TimeStart) +
                                 "\nГод окончания обработки: " + QString::number(Output->YearEnd) +
                                 "\nМесяц окончания обработки: " + QString::number(Output->MonthEnd) +
                                 "\nДень окончания обработки: " + QString::number(Output->DayEnd) +
                                 "\nМосковское время (в секундах) окончания обработки: " + QString::number(Output->TimeEnd) +
                                 "\n\n\n---Справочная информация исходного массива---\n" +
                                 "\nДлина всего массива: " + QString::number(Output->SizeTmypl) +
                                 "\nДлина комплекта исходных данных: " + QString::number(Output->lengthSD) +
                                 "\nДлина массива исходной информации: " + QString::number(Output->lengthII) +
                                 "\nОбщее количество информационных групп: " + QString::number(Output->kolIG) +
                                 "\nОбщее количество информационных блоков: " + QString::number(Output->kolIB) +
                                 "\nКоличество групп со значением Мнди = 1: " + QString::number(Output->Mndi) +
                                 "\nКоличество групп со значением Мн = 1: " + QString::number(Output->Mn) +
                                 "\n\n\n---Учетная информация выходного массива---\n" +
                                 "\nНомер сеанса: " + QString::number(Output->SesN) +
                                 "\nГод создания сеанса: " + QString::number(Output->YearSes) +
                                 "\nМесяц создания сеанса: " + QString::number(Output->MonthSes) +
                                 "\nДень создания сеанса: " + QString::number(Output->DaySes) +
                                 "\nЧисло часов на момент создания сеанса: " + QString::number(Output->chasSes) +
                                 "\nЧисло минут на момент создания сеанса: " + QString::number(Output->minSes) +
                                 "\nЧисло секунд на момент создания сеанса: " + QString::number(Output->secSes) +
                                 "\nУчётный номер комплекта исходных данных: " + QString::number(Output->YNsdSes) +
                                 "\n\n\n---Справочная информация выходного массива---\n" +
                                 "\nДлина сеанса в исходном массиве: " + QString::number(Output->lengthSes) +
                                 "\nОбщее количество информационных групп в сеансе: " + QString::number(Output->kolIGses) +
                                 "\nОбщее количество информационных блоков в сеансе: " + QString::number(Output->kolIBses) +
                                 "\nКоличество групп со значением Мнди = 1 в сеансе: " + QString::number(Output->MndiSes) +
                                 "\nКоличество групп со значением Мн = 1 в сеансе: " + QString::number(Output->MnSes) +
                                 "\n\nИсходные данные по сеансу включения №" + QString::number(Output->SesN) +
                                 "\nНомер режима работы изделия 14В18 в сеансе: " + QString::number(ID1ses[ComboIndex].rrs) +
                                 "\nПризнак потребителя: " + QString::number(ID1ses[ComboIndex].ps) +
                                 "\nГод начала сеанса: " + QString::number(ID1ses[ComboIndex].Year) +
                                 "\nМесяц начала сеанса: " + QString::number(ID1ses[ComboIndex].Month) +
                                 "\nДень начала сеанса: " + QString::number(ID1ses[ComboIndex].Day) +
                                 "\nЧисло суток в пределах текущего четырехлетия на момент начала сеанса: " + QString::number(ID1ses[ComboIndex].sutki) +
                                 "\nЧисло четырехлетий (с 1-го января 1996г.) на момент начала сеанса: " + QString::number(ID1ses[ComboIndex].nlet4) +
                                 "\nЧисло часов на момент начала сеанса: " + QString::number(ID1ses[ComboIndex].chas) +
                                 "\nЧисло минут на момент начала сеанса: " + QString::number(ID1ses[ComboIndex].min) +
                                 "\nЧисло секунд на момент начала сеанса: " + QString::number(ID1ses[ComboIndex].sec) +
                                 "\nДлительность сеанса [сек]: " + QString::number(ID1ses[ComboIndex].dt) +
                                 "\nКоординаты начала сеанса - широта [рад]: " + QString("%1").arg(ID1ses[ComboIndex].bn, 0, 'f', 6) +
                                 "\nКоординаты начала сеанса - долгота [рад]: " + QString("%1").arg(ID1ses[ComboIndex].ln, 0, 'f', 6) +
                                 "\nКоординаты конца сеанса - широта [рад]: " + QString("%1").arg(ID1ses[ComboIndex].bk, 0, 'f', 6) +
                                 "\nКоординаты конца сеанса - долгота [рад]: " + QString("%1").arg(ID1ses[ComboIndex].lk, 0, 'f', 6) +
                                 "\nДолгота узла [рад]: " + QString("%1").arg(Session->uz, 0, 'f', 6) +
                                 "\nПеречень номеров районов: " + QString("%1").arg(ID1ses[ComboIndex].nr, 32, 2, QChar('0')) +
                                 "\nТекущий номер витка, на котором производится включение: " + QString::number(ID1ses[ComboIndex].nv) +
                                 "\nНомер суточный витка включения: " + QString::number(ID1ses[ComboIndex].nd) +
                                 "\nВремя прохождения узла [сек]: " + QString("%1").arg(ID1ses[ComboIndex].tuz, 0, 'f', 6) +
                                 "\nВремя сеанса от узла [сек]: " + QString("%1").arg(ID1ses[ComboIndex].dt0, 0, 'f', 6) +
                                 "\nУгол наклонения [рад]: " + QString("%1").arg(ID1ses[ComboIndex].i, 0, 'f', 6) + "\n");

        k = -1;
        for(j = 0; j <= 24; j++)
        {
            k++;
            if (j <= 7)
                Outtxt.append("\n ax" + QString::number(k) + " = " + QString("%1").arg(ID1ses[ComboIndex].x[j], 0, 'f', 6));
            else
                if (j <= 15)
                    Outtxt.append("\n by" + QString::number(k) + " = " + QString("%1").arg(ID1ses[ComboIndex].x[j], 0, 'f', 6));
                else
                    if (j <= 23)
                        Outtxt.append("\n cz" + QString::number(k) + " = " + QString("%1").arg(ID1ses[ComboIndex].x[j], 0, 'f', 6));
                    else
                        Outtxt.append("\n t ср (сек) = " + QString("%1").arg(ID1ses[ComboIndex].x[j], 0, 'f', 6));
            if (k==7)
            {
                k = -1;
                Outtxt.append("\n");
            }
        }

        Outtxt.append("\n\nМАССИВ РЕЖИМОВ ИЗДЕЛИЯ 14В18 В СЕАНСЕ:");
        Outtxt.append("\nПредставление массива режимов в виде 16-ричных слов");
        for(j = 0; j < 10; j++)
        {
            if (j == 9)
                Outtxt.append("\n СД " + QString::number(j+1) + ": " + QString("%1").arg(ID1ses[ComboIndex].mr[j], 16, 2, QChar('0')));
            else
                Outtxt.append("\n СД  " + QString::number(j+1) + ": " + QString("%1").arg(ID1ses[ComboIndex].mr[j], 16, 2, QChar('0')));

        }

        Outtxt.append("\nПризнак совместной обработки с КАПС 'О': " + QString::number(ID1ses[ComboIndex].psop) +
                      "\nЗначение периода обращения: " + QString::number(ID1ses[ComboIndex].too) + "\n" +
                      "\nКоличество выходных формуляров МНИ: " + QString::number(Output->kolMNIses) +
                      "\nСмещение (в байтах от начала файла) области, где расположены выходные формуляры МНИ: " + QString::number(Output->offset));

        for(j = 0; j < Output->kolMNIses; j++)
        {                                   // Цикл записи формуляров МНИ
            Outtxt.append("\n\nФМНИ №" + QString::number(j+1) + ":" +
                          "\nТип сообщения: " + QString::number(FMNIses[ComboIndex][j].TypeMes) +
                          "\nКоличество групп: " + QString::number(FMNIses[ComboIndex][j].qGroup) +
                          "\nНомер массива МНИ: " + QString::number(FMNIses[ComboIndex][j].NumMas) +
                          "\nТекущее четырёхлетие: " + QString::number(FMNIses[ComboIndex][j].cMV4l) +
                          "\nТекущее число суток: " + QString::number(FMNIses[ComboIndex][j].cMVday) +
                          "\nТекущее число часов: " + QString::number(FMNIses[ComboIndex][j].cMVhour) +
                          "\nТекущее число минут: " + QString::number(FMNIses[ComboIndex][j].cMVmin) +
                          "\nТекущее число секунд: " + QString::number(FMNIses[ComboIndex][j].cMVsec) +
                          "\nКоэффициент A0x: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA0X, -15, 10, QChar(' ')) +
                          "Коэффициент A0y: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA0Y, -15, 10, QChar(' ')) +
                          "Коэффициент A0z: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA0Z, -15, 10, QChar(' ')) +
                          "\nКоэффициент A1x: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA1X, -15, 10, QChar(' ')) +
                          "Коэффициент A1y: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA1Y, -15, 10, QChar(' ')) +
                          "Коэффициент A1z: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA1Z, -15, 10, QChar(' ')) +
                          "\nКоэффициент A2x: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA2X, -15, 10, QChar(' ')) +
                          "Коэффициент A2y: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA2Y, -15, 10, QChar(' ')) +
                          "Коэффициент A2z: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA2Z, -15, 10, QChar(' ')) +
                          "\nКонтрольная сумма формуляра МНИ: " + QString::number(FMNIses[ComboIndex][j].KSMNI) +
                          "\nРассчитанная контрольная сумма формуляра МНИ: " + QString::number(FMNIses[ComboIndex][j].KS));
        }

        QFile filetxt(Path);
        if (filetxt.open(QFile::WriteOnly | QFile::Text))
        {                                   // Условие записи информации в файл
            filetxt.write(Outtxt.toUtf8());
        }
        filetxt.close();
        QMessageBox::information(this, "Сохранение", "Файл выходного набора успешно сохранён\n\nПуть: " + SetSave->value("changed/dirOut").toString());
    }
}

/* on_pushButtonOpenFileOut_clicked() - распаковка файла выходного
*  набора и вывод данных на экран.
*  Локальные переменные:
*       PathF - путь к выбранному файлу выходного набора;
*       j - счётчик для цикла;
*       k - счётчик для осей коэффициента аппроксимирующего полинома;
*       file - файл для чтения выходного набора.
*/

void MainWindow::on_pushButtonOpenFileOut_clicked()
{
    QString PathF = QFileDialog::getOpenFileName(this, "Открыть файл выходного набора", QDir::homePath() + "/Desktop/", "rezult*.bin");

    if (PathF == "")                        // Условие проверки открытия и существования файла
    {
        QMessageBox::critical(this, "Ошибка", "Файл не был открыт или его не существует");
    }
    else
    {
        ui->textEdit->setEnabled(1);
        ui->textEdit->clear();

        memset(Output, 0, sizeof (OutputSet_type));
        memset(Session, 0, sizeof (ID1));   // Очистка структур
        int j, k;

        FILE *file = fopen(PathF.toLocal8Bit(), "r+b");
                                            // Открытие файла
        fread(Output, sizeof (OutputSet_type), 1, file);
        fread(Session, sizeof (ID1), 1, file);
        fclose(file);

        ui->textEdit->setText("Выходной набор " + QString(Output->NameMas) +
                              "\n\n\n---Учетная информация исходного массива---\n" +
                              "\nУчётный номер комплекта исходных данных: " + QString::number(Output->YNsd) +
                              "\nКоличество сеансов в исходном массиве: " + QString::number(Output->n) +
                              "\nГод начала обработки: " + QString::number(Output->YearStart) +
                              "\nМесяц начала обработки: " + QString::number(Output->MonthStart) +
                              "\nДень начала обработки: " + QString::number(Output->DayStart) +
                              "\nВремя (в секундах) начала обработки: " + QString::number(Output->TimeStart) +
                              "\nГод окончания обработки: " + QString::number(Output->YearEnd) +
                              "\nМесяц окончания обработки: " + QString::number(Output->MonthEnd) +
                              "\nДень окончания обработки: " + QString::number(Output->DayEnd) +
                              "\nВремя (в секундах) окончания обработки: " + QString::number(Output->TimeEnd) +
                              "\n\n\n---Справочная информация исходного массива---\n" +
                              "\nДлина всего массива: " + QString::number(Output->SizeTmypl) +
                              "\nДлина комплекта исходных данных: " + QString::number(Output->lengthSD) +
                              "\nДлина массива исходной информации: " + QString::number(Output->lengthII) +
                              "\nОбщее количество информационных групп: " + QString::number(Output->kolIG) +
                              "\nОбщее количество информационных блоков: " + QString::number(Output->kolIB) +
                              "\nКоличество групп со значением Мнди = 1: " + QString::number(Output->Mndi) +
                              "\nКоличество групп со значением Мн = 1: " + QString::number(Output->Mn) +
                              "\n\n\n---Учетная информация выходного массива---\n" +
                              "\nНомер сеанса: " + QString::number(Output->SesN) +
                              "\nГод создания сеанса: " + QString::number(Output->YearSes) +
                              "\nМесяц создания сеанса: " + QString::number(Output->MonthSes) +
                              "\nДень создания сеанса: " + QString::number(Output->DaySes) +
                              "\nЧисло часов на момент создания сеанса: " + QString::number(Output->chasSes) +
                              "\nЧисло минут на момент создания сеанса: " + QString::number(Output->minSes) +
                              "\nЧисло секунд на момент создания сеанса: " + QString::number(Output->secSes) +
                              "\nУчётный номер комплекта исходных данных: " + QString::number(Output->YNsdSes) +
                              "\n\n\n---Справочная информация выходного массива---\n" +
                              "\nДлина сеанса в исходном массиве: " + QString::number(Output->lengthSes) +
                              "\nОбщее количество информационных групп в сеансе: " + QString::number(Output->kolIGses) +
                              "\nОбщее количество информационных блоков в сеансе: " + QString::number(Output->kolIBses) +
                              "\nКоличество групп со значением Мнди = 1 в сеансе: " + QString::number(Output->MndiSes) +
                              "\nКоличество групп со значением Мн = 1 в сеансе: " + QString::number(Output->MnSes) +
                              "\n\nИсходные данные по сеансу включения №" + QString::number(Output->SesN) +
                              "\nНомер режима работы изделия 14В18 в сеансе: " + QString::number(Session->rrs) +
                              "\nПризнак потребителя: " + QString::number(Session->ps) +
                              "\nГод начала сеанса: " + QString::number(Session->Year) +
                              "\nМесяц начала сеанса: " + QString::number(Session->Month) +
                              "\nДень начала сеанса: " + QString::number(Session->Day) +
                              "\nЧисло суток в пределах текущего четырехлетия на момент начала сеанса: " + QString::number(Session->sutki) +
                              "\nЧисло четырехлетий (с 1-го января 1996г.) на момент начала сеанса: " + QString::number(Session->nlet4) +
                              "\nЧисло часов на момент начала сеанса: " + QString::number(Session->chas) +
                              "\nЧисло минут на момент начала сеанса: " + QString::number(Session->min) +
                              "\nЧисло секунд на момент начала сеанса: " + QString::number(Session->sec) +
                              "\nДлительность сеанса [сек]: " + QString::number(Session->dt) +
                              "\nКоординаты начала сеанса - широта [рад]: " + QString("%1").arg(Session->bn, 0, 'f', 6) +
                              "\nКоординаты начала сеанса - долгота [рад]: " + QString("%1").arg(Session->ln, 0, 'f', 6) +
                              "\nКоординаты конца сеанса - широта [рад]: " + QString("%1").arg(Session->bk, 0, 'f', 6) +
                              "\nКоординаты конца сеанса - долгота [рад]: " + QString("%1").arg(Session->lk, 0, 'f', 6) +
                              "\nДолгота узла [рад]: " + QString("%1").arg(Session->uz, 0, 'f', 6) +
                              "\nПеречень номеров районов: " + QString("%1").arg(Session->nr, 32, 2, QChar('0')) +
                              "\nТекущий номер витка, на котором производится включение: " + QString::number(Session->nv) +
                              "\nНомер суточный витка включения: " + QString::number(Session->nd) +
                              "\nВремя прохождения узла [сек]: " + QString("%1").arg(Session->tuz, 0, 'f', 6) +
                              "\nВремя сеанса от узла [сек]: " + QString("%1").arg(Session->dt0, 0, 'f', 6) +
                              "\nУгол наклонения [рад]: " + QString("%1").arg(Session->i, 0, 'f', 6) + "\n");
        k = -1;
        for(j = 0; j <= 24; j++)
        {
            k++;
            if (j <= 7)
                ui->textEdit->append(" ax" + QString::number(k) + " = " + QString("%1").arg(Session->x[j], 0, 'f', 6));
            else
                if (j <= 15)
                    ui->textEdit->append(" by" + QString::number(k) + " = " + QString("%1").arg(Session->x[j], 0, 'f', 6));
                else
                    if (j <= 23)
                        ui->textEdit->append(" cz" + QString::number(k) + " = " + QString("%1").arg(Session->x[j], 0, 'f', 6));
                    else
                        ui->textEdit->append(" t ср (сек) = " + QString("%1").arg(Session->x[j], 0, 'f', 6));
            if (k==7)
            {
                k = -1;
                ui->textEdit->append("");
            }
        }

        ui->textEdit->append("\nМАССИВ РЕЖИМОВ ИЗДЕЛИЯ 14В18 В СЕАНСЕ:");
        ui->textEdit->append("Представление массива режимов в виде 16-ричных слов");
        for(j = 0; j < 10; j++)
        {
            if (j == 9)
                ui->textEdit->append(" СД " + QString::number(j+1) + ": " + QString("%1").arg(Session->mr[j], 16, 2, QChar('0')));
            else
                ui->textEdit->append(" СД  " + QString::number(j+1) + ": " + QString("%1").arg(Session->mr[j], 16, 2, QChar('0')));

        }

        ui->textEdit->append("\nПризнак совместной обработки с КАПС 'О': " + QString::number(Session->psop) +
                             "\nЗначение периода обращения: " + QString::number(Session->too) + "\n" +
                             "\nКоличество выходных формуляров МНИ: " + QString::number(Output->kolMNIses) +
                             "\nСмещение (в байтах от начала файла) области, где расположены выходные формуляры МНИ: " + QString::number(Output->offset));
    }
}

/* on_pushButtonViewOut_clicked() - предпросмотр выходного набора сеанса.
*  Локальные переменные:
*       Time - время обработки выходного набора;
*       Date – дата обработки выходного набора;
*       lengthII - размер ИИ в байтах;
*       lengthSes - размер сеанса ИИ в байтах;
*       j - счётчик для цикла;
*       k - счётчик для осей коэффициента аппроксимирующего полинома.
*/

void MainWindow::on_pushButtonViewOut_clicked()
{
    ui->textEdit->setEnabled(1);
    ui->textEdit->clear();

    ComboIndex = ui->comboBox->currentIndex();
    QTime Time = QTime::currentTime();
    QDate Date = QDate::currentDate();
    unsigned long lengthII = FileSize - DataLoc->length - 5000;
    unsigned long lengthSes = End[ComboIndex] - Start[ComboIndex];
    int j, k;

    ui->textEdit->setText("Выходной набор " + Filename +
                          "\n\n\n---Учетная информация исходного массива---\n" +
                          "\nУчётный номер комплекта исходных данных: " + QString::number(DataRem->YNid) +
                          "\nКоличество сеансов в исходном массиве: " + QString::number(DataLoc->n) +
                          "\nГод начала обработки: " + QString::number(DataRem->Year) +
                          "\nМесяц начала обработки: " + QString::number(DataRem->Month) +
                          "\nДень начала обработки: " + QString::number(DataRem->Day) +
                          "\nМосковское время (в секундах) начала обработки: не обработано" +
                          "\nГод окончания обработки: " + QString::number(Date.year()) +
                          "\nМесяц окончания обработки: " + QString::number(Date.month()) +
                          "\nДень окончания обработки: " + QString::number(Date.day()) +
                          "\nМосковское время (в секундах) окончания обработки: не обработано" +
                          "\n\n\n---Справочная информация исходного массива---\n" +
                          "\nДлина всего массива: " + QString::number(FileSize) +
                          "\nДлина комплекта исходных данных: " + QString::number(DataLoc->length) +
                          "\nДлина массива исходной информации: " + QString::number(lengthII) +
                          "\nОбщее количество информационных групп: " + QString::number(qCeil((float)lengthII / 8)) +
                          "\nОбщее количество информационных блоков: " + QString::number(qCeil((float)lengthII / 4096)) +
                          "\nКоличество групп со значением Мнди = 1: " + QString::number(Mndi) +
                          "\nКоличество групп со значением Мн = 1: " + QString::number(Mn) +
                          "\n\n\n---Учетная информация выходного массива---\n" +
                          "\nНомер сеанса: " + QString::number(ComboIndex + 1) +
                          "\nГод создания сеанса: " + QString::number(Date.year()) +
                          "\nМесяц создания сеанса: " + QString::number(Date.month()) +
                          "\nДень создания сеанса: " + QString::number(Date.day()) +
                          "\nЧисло часов на момент создания сеанса: " + QString::number(Time.hour()) +
                          "\nЧисло минут на момент создания сеанса: " + QString::number(Time.minute()) +
                          "\nЧисло секунд на момент создания сеанса: " + QString::number(Time.second()) +
                          "\nУчётный номер комплекта исходных данных: " + QString::number(DataRem->YNid) +
                          "\n\n\n---Справочная информация выходного массива---\n" +
                          "\nДлина сеанса в исходном массиве: " + QString::number(lengthSes) +
                          "\nОбщее количество информационных групп в сеансе: " + QString::number(qCeil((float)lengthSes / 8)) +
                          "\nОбщее количество информационных блоков в сеансе: " + QString::number(qCeil((float)lengthSes / 4096)) +
                          "\nКоличество групп со значением Мнди = 1 в сеансе: " + QString::number(MndiSes[ComboIndex]) +
                          "\nКоличество групп со значением Мн = 1 в сеансе: " + QString::number(MnSes[ComboIndex]) +
                          "\n\nИсходные данные по сеансу включения №" + QString::number(ComboIndex+1) +
                          "\nНомер режима работы изделия 14В18 в сеансе: " + QString::number(ID1ses[ComboIndex].rrs) +
                          "\nПризнак потребителя: " + QString::number(ID1ses[ComboIndex].ps) +
                          "\nГод начала сеанса: " + QString::number(ID1ses[ComboIndex].Year) +
                          "\nМесяц начала сеанса: " + QString::number(ID1ses[ComboIndex].Month) +
                          "\nДень начала сеанса: " + QString::number(ID1ses[ComboIndex].Day) +
                          "\nЧисло суток в пределах текущего четырехлетия на момент начала сеанса: " + QString::number(ID1ses[ComboIndex].sutki) +
                          "\nЧисло четырехлетий (с 1-го января 1996г.) на момент начала сеанса: " + QString::number(ID1ses[ComboIndex].nlet4) +
                          "\nЧисло часов на момент начала сеанса: " + QString::number(ID1ses[ComboIndex].chas) +
                          "\nЧисло минут на момент начала сеанса: " + QString::number(ID1ses[ComboIndex].min) +
                          "\nЧисло секунд на момент начала сеанса: " + QString::number(ID1ses[ComboIndex].sec) +
                          "\nДлительность сеанса [сек]: " + QString::number(ID1ses[ComboIndex].dt) +
                          "\nКоординаты начала сеанса - широта [рад]: " + QString("%1").arg(ID1ses[ComboIndex].bn, 0, 'f', 6) +
                          "\nКоординаты начала сеанса - долгота [рад]: " + QString("%1").arg(ID1ses[ComboIndex].ln, 0, 'f', 6) +
                          "\nКоординаты конца сеанса - широта [рад]: " + QString("%1").arg(ID1ses[ComboIndex].bk, 0, 'f', 6) +
                          "\nКоординаты конца сеанса - долгота [рад]: " + QString("%1").arg(ID1ses[ComboIndex].lk, 0, 'f', 6) +
                          "\nДолгота узла [рад]: " + QString("%1").arg(ID1ses[ComboIndex].uz, 0, 'f', 6) +
                          "\nПеречень номеров районов: " + QString("%1").arg(ID1ses[ComboIndex].nr, 32, 2, QChar('0')) +
                          "\nТекущий номер витка, на котором производится включение: " + QString::number(ID1ses[ComboIndex].nv) +
                          "\nНомер суточный витка включения: " + QString::number(ID1ses[ComboIndex].nd) +
                          "\nВремя прохождения узла [сек]: " + QString("%1").arg(ID1ses[ComboIndex].tuz, 0, 'f', 6) +
                          "\nВремя сеанса от узла [сек]: " + QString("%1").arg(ID1ses[ComboIndex].dt0, 0, 'f', 6) +
                          "\nУгол наклонения [рад]: " + QString("%1").arg(ID1ses[ComboIndex].i, 0, 'f', 6) + "\n");
    k = -1;
    for(j = 0; j <= 24; j++)
    {
        k++;
        if (j <= 7)
            ui->textEdit->append(" ax" + QString::number(k) + " = " + QString("%1").arg(ID1ses[ComboIndex].x[j], 0, 'f', 6));
        else
            if (j <= 15)
                ui->textEdit->append(" by" + QString::number(k) + " = " + QString("%1").arg(ID1ses[ComboIndex].x[j], 0, 'f', 6));
            else
                if (j <= 23)
                    ui->textEdit->append(" cz" + QString::number(k) + " = " + QString("%1").arg(ID1ses[ComboIndex].x[j], 0, 'f', 6));
                else
                    ui->textEdit->append(" t ср (сек) = " + QString("%1").arg(ID1ses[ComboIndex].x[j], 0, 'f', 6));
        if (k==7)
        {
            k = -1;
            ui->textEdit->append("");
        }
    }

    ui->textEdit->append("\nМАССИВ РЕЖИМОВ ИЗДЕЛИЯ 14В18 В СЕАНСЕ:");
    ui->textEdit->append("Представление массива режимов в виде 16-ричных слов");
    for(j = 0; j < 10; j++)
    {
        if (j == 9)
            ui->textEdit->append(" СД " + QString::number(j+1) + ": " + QString("%1").arg(ID1ses[ComboIndex].mr[j], 16, 2, QChar('0')));
        else
            ui->textEdit->append(" СД  " + QString::number(j+1) + ": " + QString("%1").arg(ID1ses[ComboIndex].mr[j], 16, 2, QChar('0')));

    }

    ui->textEdit->append("\nПризнак совместной обработки с КАПС 'О': " + QString::number(ID1ses[ComboIndex].psop) +
                         "\nЗначение периода обращения: " + QString::number(ID1ses[ComboIndex].too) + "\n" +
                         "\nКоличество выходных формуляров МНИ: " + QString::number(quantityFormular(ComboIndex)) +
                         "\nСмещение (в байтах от начала файла) области, где расположены выходные формуляры МНИ: " + QString::number(Start[ComboIndex]));

    for(j = 0; j < (int)quantityFormular(ComboIndex); j++)
    {                                       // Цикл вывода формуляров МНИ
        ui->textEdit->append("\nФМНИ №" + QString::number(j+1) + ":" +
                             "\nТип сообщения: " + QString::number(FMNIses[ComboIndex][j].TypeMes) +
                             "\nКоличество групп: " + QString::number(FMNIses[ComboIndex][j].qGroup) +
                             "\nНомер массива МНИ: " + QString::number(FMNIses[ComboIndex][j].NumMas) +
                             "\nТекущее четырёхлетие: " + QString::number(FMNIses[ComboIndex][j].cMV4l) +
                             "\nТекущее число суток: " + QString::number(FMNIses[ComboIndex][j].cMVday) +
                             "\nТекущее число часов: " + QString::number(FMNIses[ComboIndex][j].cMVhour) +
                             "\nТекущее число минут: " + QString::number(FMNIses[ComboIndex][j].cMVmin) +
                             "\nТекущее число секунд: " + QString::number(FMNIses[ComboIndex][j].cMVsec) +
                             "\nКоэффициент A0x: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA0X, -15, 10, QChar(' ')) +
                             "Коэффициент A0y: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA0Y, -15, 10, QChar(' ')) +
                             "Коэффициент A0z: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA0Z, -15, 10, QChar(' ')) +
                             "\nКоэффициент A1x: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA1X, -15, 10, QChar(' ')) +
                             "Коэффициент A1y: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA1Y, -15, 10, QChar(' ')) +
                             "Коэффициент A1z: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA1Z, -15, 10, QChar(' ')) +
                             "\nКоэффициент A2x: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA2X, -15, 10, QChar(' ')) +
                             "Коэффициент A2y: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA2Y, -15, 10, QChar(' ')) +
                             "Коэффициент A2z: " + QString("%1").arg(FMNIses[ComboIndex][j].coefA2Z, -15, 10, QChar(' ')) +
                             "\nКонтрольная сумма формуляра МНИ: " + QString::number(FMNIses[ComboIndex][j].KSMNI) +
                             "\nРассчитанная контрольная сумма формуляра МНИ: " + QString::number(FMNIses[ComboIndex][j].KS));
    }
}

// on_actionSetup_triggered() - открытие окна настроек сохранения файлов приложения.

void MainWindow::on_actionSetup_triggered()
{
    if(SetupWindow != NULL)                 // Условие проверки на многократное
    {                                       // открытие окна
        delete SetupWindow;
        SetupWindow = NULL;
    }
    SetupWindow = new SetupDialog();
    SetupWindow->show();
}

/* on_actionGraph_triggered() - открытие окна построения графика
*  расхождений и вывода таблиц расчётов.
*/

void MainWindow::on_actionGraph_triggered()
{
    if(GraphWindow != NULL)                 // Условие проверки на многократное
    {                                       // открытие окна
        delete GraphWindow;
        GraphWindow = NULL;
    }
    GraphWindow = new GraphDialog(DataLoc, &ID1ses, &FMNIses);
    if(SetSave->value("changed/w_or_d").toString() == "w")
    {                                       // Условие проверки на выбранную
        GraphWindow->changedWhiteTheame();  // тему программы
    }
    else
    {
        if(SetSave->value("changed/w_or_d").toString() == "d")
        {
            GraphWindow->changedDarkTheame();
        }
    }
    GraphWindow->show();
}

/* on_actionWhite_triggered() - изменение темы приложения на светлую.
*  Локальная переменная:
*       i - счётчик для цикла.
*/

void MainWindow::on_actionWhite_triggered()
{
    for (int i = 0; i < 2; i++)
    {                                       // Цикл изменения темы программы
        qApp->setPalette(style()->standardPalette());
        qApp->setStyle(QStyleFactory::create("WindowsVista"));
        qApp->setStyleSheet("QToolTip {"
                            "   color: #000000;"
                            "   background-color: #FFFFFF;"
                            "   border: 1px solid black;"
                            "}"
                            "QGroupBox::!active {"
                            "   color: black;"
                            "}"
                            "QLabel::!active {"
                            "   color: black;"
                            "}");
    }
    SetSave->setValue("changed/w_or_d", "w");
    if(GraphWindow != NULL)                 // Условие проверки на многократное
    {                                       // открытие окна
        GraphWindow->changedWhiteTheame();
    }
}

/* on_actionDark_triggered() - изменение темы приложения на тёмную.
*  Локальные переменные:
*       i - счётчик для цикла;
*       darkPalette - переменная для изменения темы приложения на тёмную.
*/

void MainWindow::on_actionDark_triggered()
{
    for (int i = 0; i < 2; i++)
    {                                       // Цикл изменения темы программы
        QPalette darkPalette;
        qApp->setStyle(QStyleFactory::create("Fusion"));
        darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);
        darkPalette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
        darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
        qApp->setStyleSheet("QToolTip {"
                            "   color: #FFFFFF;"
                            "   background-color: #353535;"
                            "   border: 1px solid black;"
                            "}");
        qApp->setPalette(darkPalette);
    }
    SetSave->setValue("changed/w_or_d", "d");
    if(GraphWindow != NULL)                 // Условие проверки на многократное
    {                                       // открытие окна
        GraphWindow->changedDarkTheame();
    }
}
