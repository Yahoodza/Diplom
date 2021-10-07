// GraphDialog - модуль построения графиков расхождений

#include "graphdialog.h"
#include "ui_graphdialog.h"

/* GraphDialog() - конструктор класса.
*  Формальные параметры:
*       DataLoc - справка о расположении данных, структура SMID;
*       ID1ses - массив ИД по сеансам включений из структуры ID1;
*       FMNIses - вумерный массив данных ФМНИ по сеансам;
*       parent - объект для отображения интерфейса.
*  Локальные переменные:
*       i - счётчик для цикла по количеству сеансов;
*       j - счётчик для цикла по количеству формуляров МНИ.
*/

GraphDialog::GraphDialog(SMID *DataLoc, QVector<ID1> *ID1ses, QVector<QVector<MNI_type> > *FMNIses, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    ui->comboBox->blockSignals(true);
                                            // Настройка графиков
    ui->widgetX->xAxis->setLabelFont(QFont(QFont().family(), 12));
    ui->widgetX->yAxis->setLabelFont(QFont(QFont().family(), 12));
    ui->widgetX->xAxis->setTickLabelFont(QFont(QFont().family(), 10));
    ui->widgetX->yAxis->setTickLabelFont(QFont(QFont().family(), 10));
    ui->widgetX->legend->setVisible(true);
    ui->widgetX->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
    ui->widgetX->setInteraction(QCP::iRangeZoom, true);
    ui->widgetX->setInteraction(QCP::iRangeDrag, true);
    ui->widgetX->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    ui->widgetX->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);

    ui->widgetY->xAxis->setLabelFont(QFont(QFont().family(), 12));
    ui->widgetY->yAxis->setLabelFont(QFont(QFont().family(), 12));
    ui->widgetY->xAxis->setTickLabelFont(QFont(QFont().family(), 10));
    ui->widgetY->yAxis->setTickLabelFont(QFont(QFont().family(), 10));
    ui->widgetY->legend->setVisible(true);
    ui->widgetY->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
    ui->widgetY->setInteraction(QCP::iRangeZoom, true);
    ui->widgetY->setInteraction(QCP::iRangeDrag, true);
    ui->widgetY->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    ui->widgetY->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);

    ui->widgetZ->xAxis->setLabelFont(QFont(QFont().family(), 12));
    ui->widgetZ->yAxis->setLabelFont(QFont(QFont().family(), 12));
    ui->widgetZ->xAxis->setTickLabelFont(QFont(QFont().family(), 10));
    ui->widgetZ->yAxis->setTickLabelFont(QFont(QFont().family(), 10));
    ui->widgetZ->legend->setVisible(true);
    ui->widgetZ->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
    ui->widgetZ->setInteraction(QCP::iRangeZoom, true);
    ui->widgetZ->setInteraction(QCP::iRangeDrag, true);
    ui->widgetZ->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    ui->widgetZ->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);

    ui->widgetR->xAxis->setLabelFont(QFont(QFont().family(), 12));
    ui->widgetR->yAxis->setLabelFont(QFont(QFont().family(), 12));
    ui->widgetR->xAxis->setTickLabelFont(QFont(QFont().family(), 10));
    ui->widgetR->yAxis->setTickLabelFont(QFont(QFont().family(), 10));
    ui->widgetR->legend->setVisible(true);
    ui->widgetR->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
    ui->widgetR->setInteraction(QCP::iRangeZoom, true);
    ui->widgetR->setInteraction(QCP::iRangeDrag, true);
    ui->widgetR->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    ui->widgetR->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);

    ui->comboBox->clear();
    CalcSes.resize(DataLoc->n);
    for (int i = 0; i != DataLoc->n; i++)
    {                                       // Цикл смены сеанса
        ui->comboBox->addItem("Сеанс №" + QString::number(i+1));
        CalcSes[i].resize(FMNIses->at(i).count() - 1);
        for (int j = 0; j != FMNIses->at(i).count() - 1; j++)
        {                                   // Вложенный цикл для расчёта координат
            CalcSes[i][j].append(FMNIses->at(i).at(j+1).cMVhour);
            CalcSes[i][j].append(FMNIses->at(i).at(j+1).cMVmin);
            CalcSes[i][j].append(FMNIses->at(i).at(j+1).cMVsec);
            CalcSes[i][j].append(FMNIses->at(i).at(j+1).cMVhour * 3600 + FMNIses->at(i).at(j+1).cMVmin * 60 + FMNIses->at(i).at(j+1).cMVsec);
            CalcSes[i][j].append(CalcSes[i][j][3] - ID1ses->at(i).x[24]);
            if (CalcSes[i][j][4] > 0)
                CalcSes[i][j].append((ID1ses->at(i).x[0] +
                                     ID1ses->at(i).x[1] * 0.01 * -CalcSes[i][j][4] +
                                     ID1ses->at(i).x[2] * 0.000001 * CalcSes[i][j][4] * CalcSes[i][j][4]) / 1000.0);
            else
                CalcSes[i][j].append((ID1ses->at(i).x[0] +
                                     ID1ses->at(i).x[1] * 0.01 * CalcSes[i][j][4] +
                                     ID1ses->at(i).x[2] * 0.000001 * CalcSes[i][j][4] * CalcSes[i][j][4]) / 1000.0);
            CalcSes[i][j].append((ID1ses->at(i).x[8] +
                                 ID1ses->at(i).x[9] * 0.001 * CalcSes[i][j][4] +
                                 ID1ses->at(i).x[10] * 0.000001 * CalcSes[i][j][4] * CalcSes[i][j][4]) / 1000.0);
            CalcSes[i][j].append((ID1ses->at(i).x[16] +
                                 ID1ses->at(i).x[17] * 0.001 * CalcSes[i][j][4] +
                                 ID1ses->at(i).x[18] * 0.000001 * CalcSes[i][j][4] * CalcSes[i][j][4]) / 1000.0);

            CalcSes[i][j].append(FMNIses->at(i).at(j+1).coefA0X / 262144.0 +
                    FMNIses->at(i).at(j+1).coefA1X * -CalcSes[i][j][4] / 134217728.0 +
                    FMNIses->at(i).at(j+1).coefA2X * 0.000001 * CalcSes[i][j][4] * CalcSes[i][j][4] / 67108864.0);
            CalcSes[i][j].append(FMNIses->at(i).at(j+1).coefA0Y / 262144.0 +
                    FMNIses->at(i).at(j+1).coefA1Y * -CalcSes[i][j][4] / 134217728.0 +
                    FMNIses->at(i).at(j+1).coefA2Y * 0.000001 * CalcSes[i][j][4] * CalcSes[i][j][4] / 67108864.0);
            CalcSes[i][j].append(FMNIses->at(i).at(j+1).coefA0Z / 262144.0 +
                    FMNIses->at(i).at(j+1).coefA1Z * -CalcSes[i][j][4] / 134217728.0 +
                    FMNIses->at(i).at(j+1).coefA2Z * 0.000001 * CalcSes[i][j][4] * CalcSes[i][j][4] / 67108864.0);

            CalcSes[i][j].append(CalcSes[i][j][8] - CalcSes[i][j][5]);
            CalcSes[i][j].append(CalcSes[i][j][11] * CalcSes[i][j][11]);
            CalcSes[i][j].append(CalcSes[i][j][9] - CalcSes[i][j][6]);
            CalcSes[i][j].append(CalcSes[i][j][13] * CalcSes[i][j][13]);
            CalcSes[i][j].append(CalcSes[i][j][10] - CalcSes[i][j][7]);
            CalcSes[i][j].append(CalcSes[i][j][15] * CalcSes[i][j][15]);
            CalcSes[i][j].append(CalcSes[i][j][12] + CalcSes[i][j][14] + CalcSes[i][j][16]);
            CalcSes[i][j].append(qSqrt(CalcSes[i][j][17]));
        }
    }
    ui->comboBox->blockSignals(false);
    on_comboBox_currentIndexChanged(0);
}

// ~GraphDialog() – деструктор класса.

GraphDialog::~GraphDialog()
{
    delete ui;
}

// changedWhiteTheame() - светлая тема графиков.

void GraphDialog::changedWhiteTheame()
{                                           // Изменение палитры цвета графиков на светлую
    ui->widgetX->setBackground(QColor("#FFFFFF"));
    ui->widgetX->xAxis->setBasePen(QColor("#000000"));
    ui->widgetX->xAxis->setTickPen(QColor("#000000"));
    ui->widgetX->xAxis->setSubTickPen(QColor("#000000"));
    ui->widgetX->xAxis->setTickLabelColor(QColor("#000000"));
    ui->widgetX->xAxis->setLabelColor(QColor("#000000"));
    ui->widgetX->yAxis->setBasePen(QColor("#000000"));
    ui->widgetX->yAxis->setTickPen(QColor("#000000"));
    ui->widgetX->yAxis->setSubTickPen(QColor("#000000"));
    ui->widgetX->yAxis->setTickLabelColor(QColor("#000000"));
    ui->widgetX->yAxis->setLabelColor(QColor("#000000"));
    ui->widgetX->legend->setBrush(QColor("#FFFFFF"));
    ui->widgetX->legend->setBorderPen(QPen(QColor("#000000")));
    ui->widgetX->legend->setTextColor(QColor("#000000"));

    ui->widgetY->setBackground(QColor("#FFFFFF"));
    ui->widgetY->xAxis->setBasePen(QColor("#000000"));
    ui->widgetY->xAxis->setTickPen(QColor("#000000"));
    ui->widgetY->xAxis->setSubTickPen(QColor("#000000"));
    ui->widgetY->xAxis->setTickLabelColor(QColor("#000000"));
    ui->widgetY->xAxis->setLabelColor(QColor("#000000"));
    ui->widgetY->yAxis->setBasePen(QColor("#000000"));
    ui->widgetY->yAxis->setTickPen(QColor("#000000"));
    ui->widgetY->yAxis->setSubTickPen(QColor("#000000"));
    ui->widgetY->yAxis->setTickLabelColor(QColor("#000000"));
    ui->widgetY->yAxis->setLabelColor(QColor("#000000"));
    ui->widgetY->legend->setBrush(QColor("#FFFFFF"));
    ui->widgetY->legend->setBorderPen(QPen(QColor("#000000")));
    ui->widgetY->legend->setTextColor(QColor("#000000"));

    ui->widgetZ->setBackground(QColor("#FFFFFF"));
    ui->widgetZ->xAxis->setBasePen(QColor("#000000"));
    ui->widgetZ->xAxis->setTickPen(QColor("#000000"));
    ui->widgetZ->xAxis->setSubTickPen(QColor("#000000"));
    ui->widgetZ->xAxis->setTickLabelColor(QColor("#000000"));
    ui->widgetZ->xAxis->setLabelColor(QColor("#000000"));
    ui->widgetZ->yAxis->setBasePen(QColor("#000000"));
    ui->widgetZ->yAxis->setTickPen(QColor("#000000"));
    ui->widgetZ->yAxis->setSubTickPen(QColor("#000000"));
    ui->widgetZ->yAxis->setTickLabelColor(QColor("#000000"));
    ui->widgetZ->yAxis->setLabelColor(QColor("#000000"));
    ui->widgetZ->legend->setBrush(QColor("#FFFFFF"));
    ui->widgetZ->legend->setBorderPen(QPen(QColor("#000000")));
    ui->widgetZ->legend->setTextColor(QColor("#000000"));

    ui->widgetR->setBackground(QColor("#FFFFFF"));
    ui->widgetR->xAxis->setBasePen(QColor("#000000"));
    ui->widgetR->xAxis->setTickPen(QColor("#000000"));
    ui->widgetR->xAxis->setSubTickPen(QColor("#000000"));
    ui->widgetR->xAxis->setTickLabelColor(QColor("#000000"));
    ui->widgetR->xAxis->setLabelColor(QColor("#000000"));
    ui->widgetR->yAxis->setBasePen(QColor("#000000"));
    ui->widgetR->yAxis->setTickPen(QColor("#000000"));
    ui->widgetR->yAxis->setSubTickPen(QColor("#000000"));
    ui->widgetR->yAxis->setTickLabelColor(QColor("#000000"));
    ui->widgetR->yAxis->setLabelColor(QColor("#000000"));
    ui->widgetR->legend->setBrush(QColor("#FFFFFF"));
    ui->widgetR->legend->setBorderPen(QPen(QColor("#000000")));
    ui->widgetR->legend->setTextColor(QColor("#000000"));
}

// changedDarkTheame() - тёмная тема графиков.

void GraphDialog::changedDarkTheame()
{                                           // Изменение палитры цвета графиков на тёмную
    ui->widgetX->setBackground(QColor(25, 25, 25));
    ui->widgetX->xAxis->setBasePen(QColor("#FFFFFF"));
    ui->widgetX->xAxis->setTickPen(QColor("#FFFFFF"));
    ui->widgetX->xAxis->setSubTickPen(QColor("#FFFFFF"));
    ui->widgetX->xAxis->setTickLabelColor(QColor("#FFFFFF"));
    ui->widgetX->xAxis->setLabelColor(QColor("#FFFFFF"));
    ui->widgetX->yAxis->setBasePen(QColor("#FFFFFF"));
    ui->widgetX->yAxis->setTickPen(QColor("#FFFFFF"));
    ui->widgetX->yAxis->setSubTickPen(QColor("#FFFFFF"));
    ui->widgetX->yAxis->setTickLabelColor(QColor("#FFFFFF"));
    ui->widgetX->yAxis->setLabelColor(QColor("#FFFFFF"));
    ui->widgetX->legend->setBrush(QColor(53, 53, 53));
    ui->widgetX->legend->setBorderPen(QPen(QColor("#000000")));
    ui->widgetX->legend->setTextColor(QColor("#FFFFFF"));

    ui->widgetY->setBackground(QColor(25, 25, 25));
    ui->widgetY->xAxis->setBasePen(QColor("#FFFFFF"));
    ui->widgetY->xAxis->setTickPen(QColor("#FFFFFF"));
    ui->widgetY->xAxis->setSubTickPen(QColor("#FFFFFF"));
    ui->widgetY->xAxis->setTickLabelColor(QColor("#FFFFFF"));
    ui->widgetY->xAxis->setLabelColor(QColor("#FFFFFF"));
    ui->widgetY->yAxis->setBasePen(QColor("#FFFFFF"));
    ui->widgetY->yAxis->setTickPen(QColor("#FFFFFF"));
    ui->widgetY->yAxis->setSubTickPen(QColor("#FFFFFF"));
    ui->widgetY->yAxis->setTickLabelColor(QColor("#FFFFFF"));
    ui->widgetY->yAxis->setLabelColor(QColor("#FFFFFF"));
    ui->widgetY->legend->setBrush(QColor(53, 53, 53));
    ui->widgetY->legend->setBorderPen(QPen(QColor("#000000")));
    ui->widgetY->legend->setTextColor(QColor("#FFFFFF"));

    ui->widgetZ->setBackground(QColor(25, 25, 25));
    ui->widgetZ->xAxis->setBasePen(QColor("#FFFFFF"));
    ui->widgetZ->xAxis->setTickPen(QColor("#FFFFFF"));
    ui->widgetZ->xAxis->setSubTickPen(QColor("#FFFFFF"));
    ui->widgetZ->xAxis->setTickLabelColor(QColor("#FFFFFF"));
    ui->widgetZ->xAxis->setLabelColor(QColor("#FFFFFF"));
    ui->widgetZ->yAxis->setBasePen(QColor("#FFFFFF"));
    ui->widgetZ->yAxis->setTickPen(QColor("#FFFFFF"));
    ui->widgetZ->yAxis->setSubTickPen(QColor("#FFFFFF"));
    ui->widgetZ->yAxis->setTickLabelColor(QColor("#FFFFFF"));
    ui->widgetZ->yAxis->setLabelColor(QColor("#FFFFFF"));
    ui->widgetZ->legend->setBrush(QColor(53, 53, 53));
    ui->widgetZ->legend->setBorderPen(QPen(QColor("#000000")));
    ui->widgetZ->legend->setTextColor(QColor("#FFFFFF"));

    ui->widgetR->setBackground(QColor(25, 25, 25));
    ui->widgetR->xAxis->setBasePen(QColor("#FFFFFF"));
    ui->widgetR->xAxis->setTickPen(QColor("#FFFFFF"));
    ui->widgetR->xAxis->setSubTickPen(QColor("#FFFFFF"));
    ui->widgetR->xAxis->setTickLabelColor(QColor("#FFFFFF"));
    ui->widgetR->xAxis->setLabelColor(QColor("#FFFFFF"));
    ui->widgetR->yAxis->setBasePen(QColor("#FFFFFF"));
    ui->widgetR->yAxis->setTickPen(QColor("#FFFFFF"));
    ui->widgetR->yAxis->setSubTickPen(QColor("#FFFFFF"));
    ui->widgetR->yAxis->setTickLabelColor(QColor("#FFFFFF"));
    ui->widgetR->yAxis->setLabelColor(QColor("#FFFFFF"));
    ui->widgetR->legend->setBrush(QColor(53, 53, 53));
    ui->widgetR->legend->setBorderPen(QPen(QColor("#000000")));
    ui->widgetR->legend->setTextColor(QColor("#FFFFFF"));
}

/* on_comboBox_currentIndexChanged() – событие изменения сеанса.
*  Локальные переменные:
*       cell - элемент tableWidget для записи данных в таблицы;
*       Xsd – массив координат по X из ИД;
*       Xm – массив координат по X из МНИ;
*       Ysd – массив координат Y из ИД;
*       Ym – массив координат Y из МНИ;
*       Zsd - массив координат Z из ИД;
*       Zm - массив координат Z из МНИ;
*       R - массив отклонений орбит;
*       t - массив времени пребывания КА в определённой координате;
*       Xmax - максимальный по модулю элемент массива Xm - Xsd;
*       Ymax - максимальный по модулю элемент массива Ym - Ysd;
*       Zmax - максимальный по модулю элемент массива Zm - Zsd;
*       Rmax - максимальный по модулю элемент массива R;
*       Xn - корректировка по координате X;
*       Yn - корректировка по координате Y;
*       Zn - корректировка по координате Z;
*       Rn - корректировка всей орбиты;
*       i - счётчик для цикла.
*/

void GraphDialog::on_comboBox_currentIndexChanged(int index)
{
    QTableWidgetItem *cell = new QTableWidgetItem();
    TableSD.clear();
    TableMNI.clear();
    TableDiverg.clear();

    ui->widgetX->clearGraphs();
    ui->widgetY->clearGraphs();
    ui->widgetZ->clearGraphs();
    ui->widgetR->clearGraphs();
    ui->tableWidgetSD->clear();
    ui->tableWidgetMNI->clear();
    ui->tableWidgetDiverg->clear();

                                            // Настройка таблиц расчётов
    ui->tableWidgetSD->horizontalHeader()->setVisible(true);
    ui->tableWidgetSD->setColumnCount(7);
    ui->tableWidgetSD->setHorizontalHeaderLabels(QStringList() << "Номер\nформуляра МНИ" << "Московское время ФМНИ\n(час, мин, сек)"
             << "Московское время ФМНИ\n(сек)" << "t - tср\n(сек)" << "Xид\n(км)" << "Yид\n(км)" << "Zид\n(км)");
    ui->tableWidgetMNI->horizontalHeader()->setVisible(true);
    ui->tableWidgetMNI->setColumnCount(7);
    ui->tableWidgetMNI->setHorizontalHeaderLabels(QStringList() << "Номер\nформуляра МНИ" << "Московское время ФМНИ\n(час, мин, сек)"
              << "Московское время ФМНИ\n(сек)" << "tср - t\n(сек)" << "Xм\n(км)" << "Yм\n(км)" << "Zм\n(км)");
    ui->tableWidgetDiverg->horizontalHeader()->setVisible(true);
    ui->tableWidgetDiverg->setColumnCount(11);
    ui->tableWidgetDiverg->setHorizontalHeaderLabels(QStringList() << "Номер\nформуляра МНИ" << "Московское время ФМНИ\n(час, мин, сек)"
                 << "Московское время ФМНИ\n(сек)" << "Xм - Xид\n(км)" << "(Xм - Xид)²\n(км)²"
                 << "Yм - Yид\n(км)" << "(Yм - Yид)²\n(км)²" << "Zм - Zид\n(км)" << "(Zм - Zид)²\n(км)²"
                 << "(Xм - Xид)² +\n+ (Yм - Yид)² +\n+ (Zм - Zид)²\n(км)²" << "R\n(км)");

    ui->tableWidgetSD->setRowCount(CalcSes[index].count());
    ui->tableWidgetMNI->setRowCount(CalcSes[index].count());
    ui->tableWidgetDiverg->setRowCount(CalcSes[index].count() + 3);
    QVector<double> Xsd(CalcSes[index].count()),
                    Xm(CalcSes[index].count()),
                    Ysd(CalcSes[index].count()),
                    Ym(CalcSes[index].count()),
                    Zsd(CalcSes[index].count()),
                    Zm(CalcSes[index].count()),
                    R(CalcSes[index].count()),
                    t(CalcSes[index].count());
    double Xmax = 0, Ymax = 0, Zmax = 0, Rmax = 0;
    QString Xn, Yn, Zn, Rn;

                                            // Создание таблиц расчётов для текстового файла
    TableSD = QString("Расчет значений координат центра масс КА по данным полинома Чебышева из ИД\n\n"
                      "╔═══════╦══════════════════╦══════════════════╦═════════╦══════════════╦══════════════╦══════════════╗\n"
                      "║       ║    Московское    ║    Московское    ║         ║              ║              ║              ║\n"
                      "║ Номер ║ время в исходном ║ время в исходном ║ t - tср ║      Xид     ║      Yид     ║      Zид     ║\n"
                      "║  МНИ  ║   формуляре МНИ  ║   формуляре МНИ  ║  (сек)  ║      (км)    ║      (км)    ║      (км)    ║\n"
                      "║       ║  (час, мин, сек) ║       (сек)      ║         ║              ║              ║              ║\n"
                      "╠═══════╬══════════════════╬══════════════════╬═════════╬══════════════╬══════════════╬══════════════╣\n");

    TableMNI = QString("Расчет значений координат центра масс КА по данным исходных формуляров МНИ\n\n"
                       "╔═══════╦══════════════════╦══════════════════╦═════════╦══════════════╦══════════════╦══════════════╗\n"
                       "║       ║    Московское    ║    Московское    ║         ║              ║              ║              ║\n"
                       "║ Номер ║ время в исходном ║ время в исходном ║ tср - t ║      Xм      ║      Yм      ║      Zм      ║\n"
                       "║  МНИ  ║   формуляре МНИ  ║   формуляре МНИ  ║  (сек)  ║     (км)     ║     (км)     ║     (км)     ║\n"
                       "║       ║  (час, мин, сек) ║       (сек)      ║         ║              ║              ║              ║\n"
                       "╠═══════╬══════════════════╬══════════════════╬═════════╬══════════════╬══════════════╬══════════════╣\n");

    TableDiverg = QString("Расчет значения величины отклонения при определении координат центра масс КА\n\n"
                          "╔═══════╦══════════════════╦══════════════════╦══════════════╦══════════════╦══════════════╦══════════════╦══════════════╦══════════════╦═══════════════╦══════════════╗\n"
                          "║       ║    Московское    ║    Московское    ║              ║              ║              ║              ║              ║              ║  (Xм - Xид)²+ ║              ║\n"
                          "║ Номер ║ время в исходном ║ время в исходном ║   Xм - Xид   ║  (Xм - Xид)² ║   Yм - Yид   ║  (Yм - Yид)² ║   Zм - Zид   ║  (Zм - Zид)² ║ +(Yм - Yид)²+ ║      R       ║\n"
                          "║  МНИ  ║   формуляре МНИ  ║   формуляре МНИ  ║     (км)     ║     (км)²    ║     (км)     ║     (км)²    ║     (км)     ║     (км)²    ║  +(Zм - Zид)² ║     (км)     ║\n"
                          "║       ║  (час, мин, сек) ║       (сек)      ║              ║              ║              ║              ║              ║              ║      (км)²    ║              ║\n"
                          "╠═══════╬══════════════════╬══════════════════╬══════════════╬══════════════╬══════════════╬══════════════╬══════════════╬══════════════╬═══════════════╬══════════════╣\n");

    for (int i = 0; i != CalcSes[index].count(); i++)
    {                                       // Цикл записи данныйх в массивы координат и
        Xsd[i] = CalcSes[index][i][5];      // записи данных в таблицы расчётов
        Xm[i] = CalcSes[index][i][8];
        Ysd[i] = CalcSes[index][i][6];
        Ym[i] = CalcSes[index][i][9];
        Zsd[i] = CalcSes[index][i][7];
        Zm[i] = CalcSes[index][i][10];
        R[i] = CalcSes[index][i][18];
        t[i] = CalcSes[index][i][3];

        cell = new QTableWidgetItem(QString::number(i + 2));
        ui->tableWidgetSD->setItem(i, 0, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][0]) + "." +
                QString("%1").arg(CalcSes[index][i][1], 2, 'g', -1, QChar('0')) + "." +
                QString("%1").arg(CalcSes[index][i][2], 2, 'g', -1, QChar('0')));
        ui->tableWidgetSD->setItem(i, 1, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][3]));
        ui->tableWidgetSD->setItem(i, 2, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][4]));
        ui->tableWidgetSD->setItem(i, 3, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][5], 'f', 3));
        ui->tableWidgetSD->setItem(i, 4, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][6], 'f', 3));
        ui->tableWidgetSD->setItem(i, 5, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][7], 'f', 3));
        ui->tableWidgetSD->setItem(i, 6, cell);

        cell = new QTableWidgetItem(QString::number(i + 2));
        ui->tableWidgetMNI->setItem(i, 0, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][0]) + "." +
                QString("%1").arg(CalcSes[index][i][1], 2, 'g', -1, QChar('0')) + "." +
                QString("%1").arg(CalcSes[index][i][2], 2, 'g', -1, QChar('0')));
        ui->tableWidgetMNI->setItem(i, 1, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][3]));
        ui->tableWidgetMNI->setItem(i, 2, cell);
        cell = new QTableWidgetItem(QString::number(-CalcSes[index][i][4]));
        ui->tableWidgetMNI->setItem(i, 3, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][8], 'f', 3));
        ui->tableWidgetMNI->setItem(i, 4, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][9], 'f', 3));
        ui->tableWidgetMNI->setItem(i, 5, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][10], 'f', 3));
        ui->tableWidgetMNI->setItem(i, 6, cell);

        cell = new QTableWidgetItem(QString::number(i + 2));
        ui->tableWidgetDiverg->setItem(i, 0, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][0]) + "." +
                QString("%1").arg(CalcSes[index][i][1], 2, 'g', -1, QChar('0')) + "." +
                QString("%1").arg(CalcSes[index][i][2], 2, 'g', -1, QChar('0')));
        ui->tableWidgetDiverg->setItem(i, 1, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][3]));
        ui->tableWidgetDiverg->setItem(i, 2, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][11], 'f', 3));
        ui->tableWidgetDiverg->setItem(i, 3, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][12], 'f', 3));
        ui->tableWidgetDiverg->setItem(i, 4, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][13], 'f', 3));
        ui->tableWidgetDiverg->setItem(i, 5, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][14], 'f', 3));
        ui->tableWidgetDiverg->setItem(i, 6, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][15], 'f', 3));
        ui->tableWidgetDiverg->setItem(i, 7, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][16], 'f', 3));
        ui->tableWidgetDiverg->setItem(i, 8, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][17], 'f', 3));
        ui->tableWidgetDiverg->setItem(i, 9, cell);
        cell = new QTableWidgetItem(QString::number(CalcSes[index][i][18], 'f', 3));
        ui->tableWidgetDiverg->setItem(i, 10, cell);

        if (qFabs(CalcSes[index][i][11]) > Xmax)
            Xmax = qFabs(CalcSes[index][i][11]);
        if (qFabs(CalcSes[index][i][13]) > Ymax)
            Ymax = qFabs(CalcSes[index][i][13]);
        if (qFabs(CalcSes[index][i][15]) > Zmax)
            Zmax = qFabs(CalcSes[index][i][15]);
        if (qFabs(CalcSes[index][i][18]) > Rmax)
            Rmax = qFabs(CalcSes[index][i][18]);

        if (i != CalcSes[index].count() - 1)
        {
            TableSD.append(QString("║%1║%2║%3║%4║%5║%6║%7║\n").
                           arg(ui->tableWidgetSD->item(i, 0)->text(), 7, QChar(' ')).
                           arg(ui->tableWidgetSD->item(i, 1)->text(), 18, QChar(' ')).
                           arg(ui->tableWidgetSD->item(i, 2)->text(), 18, QChar(' ')).
                           arg(ui->tableWidgetSD->item(i, 3)->text(), 9, QChar(' ')).
                           arg(ui->tableWidgetSD->item(i, 4)->text(), 14, QChar(' ')).
                           arg(ui->tableWidgetSD->item(i, 5)->text(), 14, QChar(' ')).
                           arg(ui->tableWidgetSD->item(i, 6)->text(), 14, QChar(' ')) +
                           "╠═══════╬══════════════════╬══════════════════╬═════════╬══════════════╬══════════════╬══════════════╣\n");

            TableMNI.append(QString("║%1║%2║%3║%4║%5║%6║%7║\n").
                           arg(ui->tableWidgetMNI->item(i, 0)->text(), 7, QChar(' ')).
                           arg(ui->tableWidgetMNI->item(i, 1)->text(), 18, QChar(' ')).
                           arg(ui->tableWidgetMNI->item(i, 2)->text(), 18, QChar(' ')).
                           arg(ui->tableWidgetMNI->item(i, 3)->text(), 9, QChar(' ')).
                           arg(ui->tableWidgetMNI->item(i, 4)->text(), 14, QChar(' ')).
                           arg(ui->tableWidgetMNI->item(i, 5)->text(), 14, QChar(' ')).
                           arg(ui->tableWidgetMNI->item(i, 6)->text(), 14, QChar(' ')) +
                           "╠═══════╬══════════════════╬══════════════════╬═════════╬══════════════╬══════════════╬══════════════╣\n");

            TableDiverg.append(QString("║%1║%2║%3║%4║%5║%6║%7║%8║%9║%10║%11║\n").
                           arg(ui->tableWidgetDiverg->item(i, 0)->text(), 7, QChar(' ')).
                           arg(ui->tableWidgetDiverg->item(i, 1)->text(), 18, QChar(' ')).
                           arg(ui->tableWidgetDiverg->item(i, 2)->text(), 18, QChar(' ')).
                           arg(ui->tableWidgetDiverg->item(i, 3)->text(), 14, QChar(' ')).
                           arg(ui->tableWidgetDiverg->item(i, 4)->text(), 14, QChar(' ')).
                           arg(ui->tableWidgetDiverg->item(i, 5)->text(), 14, QChar(' ')).
                           arg(ui->tableWidgetDiverg->item(i, 6)->text(), 14, QChar(' ')).
                           arg(ui->tableWidgetDiverg->item(i, 7)->text(), 14, QChar(' ')).
                           arg(ui->tableWidgetDiverg->item(i, 8)->text(), 14, QChar(' ')).
                           arg(ui->tableWidgetDiverg->item(i, 9)->text(), 15, QChar(' ')).
                           arg(ui->tableWidgetDiverg->item(i, 10)->text(), 14, QChar(' ')) +
                           "╠═══════╬══════════════════╬══════════════════╬══════════════╬══════════════╬══════════════╬══════════════╬══════════════╬══════════════╬═══════════════╬══════════════╣\n");
        }
        else
        {
            TableSD.append(QString("║%1║%2║%3║%4║%5║%6║%7║\n").
                           arg(ui->tableWidgetSD->item(i, 0)->text(), 7, QChar(' ')).
                           arg(ui->tableWidgetSD->item(i, 1)->text(), 18, QChar(' ')).
                           arg(ui->tableWidgetSD->item(i, 2)->text(), 18, QChar(' ')).
                           arg(ui->tableWidgetSD->item(i, 3)->text(), 9, QChar(' ')).
                           arg(ui->tableWidgetSD->item(i, 4)->text(), 14, QChar(' ')).
                           arg(ui->tableWidgetSD->item(i, 5)->text(), 14, QChar(' ')).
                           arg(ui->tableWidgetSD->item(i, 6)->text(), 14, QChar(' ')) +
                           "╚═══════╩══════════════════╩══════════════════╩═════════╩══════════════╩══════════════╩══════════════╝");

            TableMNI.append(QString("║%1║%2║%3║%4║%5║%6║%7║\n").
                           arg(ui->tableWidgetMNI->item(i, 0)->text(), 7, QChar(' ')).
                           arg(ui->tableWidgetMNI->item(i, 1)->text(), 18, QChar(' ')).
                           arg(ui->tableWidgetMNI->item(i, 2)->text(), 18, QChar(' ')).
                           arg(ui->tableWidgetMNI->item(i, 3)->text(), 9, QChar(' ')).
                           arg(ui->tableWidgetMNI->item(i, 4)->text(), 14, QChar(' ')).
                           arg(ui->tableWidgetMNI->item(i, 5)->text(), 14, QChar(' ')).
                           arg(ui->tableWidgetMNI->item(i, 6)->text(), 14, QChar(' ')) +
                           "╚═══════╩══════════════════╩══════════════════╩═════════╩══════════════╩══════════════╩══════════════╝");

            TableDiverg.append(QString("║%1║%2║%3║%4║%5║%6║%7║%8║%9║%10║%11║\n").
                               arg(ui->tableWidgetDiverg->item(i, 0)->text(), 7, QChar(' ')).
                               arg(ui->tableWidgetDiverg->item(i, 1)->text(), 18, QChar(' ')).
                               arg(ui->tableWidgetDiverg->item(i, 2)->text(), 18, QChar(' ')).
                               arg(ui->tableWidgetDiverg->item(i, 3)->text(), 14, QChar(' ')).
                               arg(ui->tableWidgetDiverg->item(i, 4)->text(), 14, QChar(' ')).
                               arg(ui->tableWidgetDiverg->item(i, 5)->text(), 14, QChar(' ')).
                               arg(ui->tableWidgetDiverg->item(i, 6)->text(), 14, QChar(' ')).
                               arg(ui->tableWidgetDiverg->item(i, 7)->text(), 14, QChar(' ')).
                               arg(ui->tableWidgetDiverg->item(i, 8)->text(), 14, QChar(' ')).
                               arg(ui->tableWidgetDiverg->item(i, 9)->text(), 15, QChar(' ')).
                               arg(ui->tableWidgetDiverg->item(i, 10)->text(), 14, QChar(' ')) +
                           "╠═══════╩══════════════════╩══════════════════╬══════════════╬══════════════╬══════════════╬══════════════╬══════════════╬══════════════╩═══════════════╬══════════════╣\n");
        }
    }
    cell = new QTableWidgetItem("Величина максимального отклонения:");
    ui->tableWidgetDiverg->setItem(CalcSes[index].count(), 0, cell);
    cell = new QTableWidgetItem("Оценка:");
    ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 1, 0, cell);
    cell = new QTableWidgetItem("Вывод:");
    ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 2, 0, cell);

    cell = new QTableWidgetItem(QString::number(Xmax, 'f', 3));
    ui->tableWidgetDiverg->setItem(CalcSes[index].count(), 3, cell);
    cell = new QTableWidgetItem(QString::number(Ymax, 'f', 3));
    ui->tableWidgetDiverg->setItem(CalcSes[index].count(), 5, cell);
    cell = new QTableWidgetItem(QString::number(Zmax, 'f', 3));
    ui->tableWidgetDiverg->setItem(CalcSes[index].count(), 7, cell);
    cell = new QTableWidgetItem(QString::number(Rmax, 'f', 3));
    ui->tableWidgetDiverg->setItem(CalcSes[index].count(), 10, cell);

    if (Xmax > 30)                          // Оценка правильности посчитанной координаты
    {
        cell = new QTableWidgetItem("Не норма");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 1, 3, cell);
        cell = new QTableWidgetItem("Требуется корректировка по координате");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 2, 3, cell);
        Xn = "Требуется";
    }
    else
    {
        cell = new QTableWidgetItem("Норма");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 1, 3, cell);
        cell = new QTableWidgetItem("Не требуется корректировка по координате");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 2, 3, cell);
        Xn = "Не требуется";
    }
    if (Ymax > 30)
    {
        cell = new QTableWidgetItem("Не норма");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 1, 5, cell);
        cell = new QTableWidgetItem("Требуется корректировка по координате");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 2, 5, cell);
        Yn = "Требуется";
    }
    else
    {
        cell = new QTableWidgetItem("Норма");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 1, 5, cell);
        cell = new QTableWidgetItem("Не требуется корректировка по координате");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 2, 5, cell);
        Yn = "Не требуется";
    }
    if (Zmax > 30)
    {
        cell = new QTableWidgetItem("Не норма");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 1, 7, cell);
        cell = new QTableWidgetItem("Требуется корректировка по координате");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 2, 7, cell);
        Zn = "Требуется";
    }
    else
    {
        cell = new QTableWidgetItem("Норма");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 1, 7, cell);
        cell = new QTableWidgetItem("Не требуется корректировка по координате");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 2, 7, cell);
        Zn = "Не требуется";
    }
    if (Rmax > 50)
    {
        cell = new QTableWidgetItem("Не норма");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 1, 10, cell);
        cell = new QTableWidgetItem("Требуется корректировка всей орбиты");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 2, 10, cell);
        Rn = "Требуется";
    }
    else
    {
        cell = new QTableWidgetItem("Норма");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 1, 10, cell);
        cell = new QTableWidgetItem("Не требуется корректировка всей орбиты");
        ui->tableWidgetDiverg->setItem(CalcSes[index].count() + 2, 10, cell);
        Rn = "Не требуется";
    }

    TableDiverg.append(QString("║%1║%2║%3║%4║%5║%6║%7║%8║\n").
                   arg(ui->tableWidgetDiverg->item(CalcSes[index].count(), 0)->text(), 45, QChar(' ')).
                   arg(ui->tableWidgetDiverg->item(CalcSes[index].count(), 3)->text(), 14, QChar(' ')).
                   arg("", 14, QChar(' ')).
                   arg(ui->tableWidgetDiverg->item(CalcSes[index].count(), 5)->text(), 14, QChar(' ')).
                   arg("", 14, QChar(' ')).
                   arg(ui->tableWidgetDiverg->item(CalcSes[index].count(), 7)->text(), 14, QChar(' ')).
                   arg("", 30, QChar(' ')).
                   arg(ui->tableWidgetDiverg->item(CalcSes[index].count(), 10)->text(), 14, QChar(' ')) +
                   "╠═════════════════════════════════════════════╬══════════════╬══════════════╬══════════════╬══════════════╬══════════════╬══════════════════════════════╬══════════════╣\n" +
                   QString("║%1║%2║%3║%4║%5║%6║%7║%8║\n").
                   arg(ui->tableWidgetDiverg->item(CalcSes[index].count() + 1, 0)->text(), 45, QChar(' ')).
                   arg(ui->tableWidgetDiverg->item(CalcSes[index].count() + 1, 3)->text(), 14, QChar(' ')).
                   arg("", 14, QChar(' ')).
                   arg(ui->tableWidgetDiverg->item(CalcSes[index].count() + 1, 5)->text(), 14, QChar(' ')).
                   arg("", 14, QChar(' ')).
                   arg(ui->tableWidgetDiverg->item(CalcSes[index].count() + 1, 7)->text(), 14, QChar(' ')).
                   arg("", 30, QChar(' ')).
                   arg(ui->tableWidgetDiverg->item(CalcSes[index].count() + 1, 10)->text(), 14, QChar(' ')) +
                   "╠═════════════════════════════════════════════╬══════════════╬══════════════╬══════════════╬══════════════╬══════════════╬══════════════════════════════╬══════════════╣\n" +
                   QString("║%1║%2║%3║%4║%5║%6║%7║%8║\n").
                   arg("", 45, QChar(' ')).
                   arg(Xn, 14, QChar(' ')).
                   arg("", 14, QChar(' ')).
                   arg(Yn, 14, QChar(' ')).
                   arg("", 14, QChar(' ')).
                   arg(Zn, 14, QChar(' ')).
                   arg("", 30, QChar(' ')).
                   arg(Rn, 14, QChar(' ')) +
                   QString("║%1║%2║%3║%4║%5║%6║%7║%8║\n").
                   arg(ui->tableWidgetDiverg->item(CalcSes[index].count() + 2, 0)->text(), 45, QChar(' ')).
                   arg("корректировка", 14, QChar(' ')).
                   arg("", 14, QChar(' ')).
                   arg("корректировка", 14, QChar(' ')).
                   arg("", 14, QChar(' ')).
                   arg("корректировка", 14, QChar(' ')).
                   arg("", 30, QChar(' ')).
                   arg("корректировка", 14, QChar(' ')) +
                   QString("║%1║%2║%3║%4║%5║%6║%7║%8║\n").
                   arg("", 45, QChar(' ')).
                   arg("по координате", 14, QChar(' ')).
                   arg("", 14, QChar(' ')).
                   arg("по координате", 14, QChar(' ')).
                   arg("", 14, QChar(' ')).
                   arg("по координате", 14, QChar(' ')).
                   arg("", 30, QChar(' ')).
                   arg("всей орбиты", 14, QChar(' ')) +
                   "╚═════════════════════════════════════════════╩══════════════╩══════════════╩══════════════╩══════════════╩══════════════╩══════════════════════════════╩══════════════╝\n\n");

                                            // Построение графиков
    ui->widgetX->addGraph();
    ui->widgetX->graph(0)->setPen(QPen(QBrush(QColor("#02b906")), 2));
    ui->widgetX->graph(0)->setName("Прогнозируемое\nизменение X");
    ui->widgetX->graph(0)->setData(t, Xsd);
    ui->widgetX->addGraph();
    ui->widgetX->graph(1)->setPen(QPen(QBrush(QColor("#af0a0a")), 2));
    ui->widgetX->graph(1)->setName("Реальное\nизменение X");
    ui->widgetX->graph(1)->setData(t, Xm);
    ui->widgetX->xAxis->setLabel("t, сек");
    ui->widgetX->yAxis->setLabel("X, км");
    ui->widgetX->xAxis->setRange(*std::min_element(t.begin(), t.end()), *std::max_element(t.begin(), t.end()));
    ui->widgetX->yAxis->setRange(__min(*std::min_element(Xsd.begin(), Xsd.end()), *std::min_element(Xm.begin(), Xm.end())),
                                 __max(*std::max_element(Xsd.begin(), Xsd.end()), *std::max_element(Xm.begin(), Xm.end())));
    ui->widgetX->replot();

    ui->widgetY->addGraph();
    ui->widgetY->graph(0)->setPen(QPen(QBrush(QColor("#02b906")), 2));
    ui->widgetY->graph(0)->setName("Прогнозируемое\nизменение Y");
    ui->widgetY->graph(0)->setData(t, Ysd);
    ui->widgetY->addGraph();
    ui->widgetY->graph(1)->setPen(QPen(QBrush(QColor("#af0a0a")), 2));
    ui->widgetY->graph(1)->setName("Реальное\nизменение Y");
    ui->widgetY->graph(1)->setData(t, Ym);
    ui->widgetY->xAxis->setLabel("t, сек");
    ui->widgetY->yAxis->setLabel("Y, км");
    ui->widgetY->xAxis->setRange(*std::min_element(t.begin(), t.end()), *std::max_element(t.begin(), t.end()));
    ui->widgetY->yAxis->setRange(__min(*std::min_element(Ysd.begin(), Ysd.end()), *std::min_element(Ym.begin(), Ym.end())),
                                 __max(*std::max_element(Ysd.begin(), Ysd.end()), *std::max_element(Ym.begin(), Ym.end())));
    ui->widgetY->replot();

    ui->widgetZ->addGraph();
    ui->widgetZ->graph(0)->setPen(QPen(QBrush(QColor("#02b906")), 2));
    ui->widgetZ->graph(0)->setName("Прогнозируемое\nизменение Z");
    ui->widgetZ->graph(0)->setData(t, Zsd);
    ui->widgetZ->addGraph();
    ui->widgetZ->graph(1)->setPen(QPen(QBrush(QColor("#af0a0a")), 2));
    ui->widgetZ->graph(1)->setName("Реальное\nизменение Z");
    ui->widgetZ->graph(1)->setData(t, Zm);
    ui->widgetZ->xAxis->setLabel("t, сек");
    ui->widgetZ->yAxis->setLabel("Z, км");
    ui->widgetZ->xAxis->setRange(*std::min_element(t.begin(), t.end()), *std::max_element(t.begin(), t.end()));
    ui->widgetZ->yAxis->setRange(__min(*std::min_element(Zsd.begin(), Zsd.end()), *std::min_element(Zm.begin(), Zm.end())),
                                 __max(*std::max_element(Zsd.begin(), Zsd.end()), *std::max_element(Zm.begin(), Zm.end())));
    ui->widgetZ->replot();

    ui->widgetR->addGraph();
    ui->widgetR->graph(0)->setPen(QPen(QBrush(QColor("#aa09db")), 2));
    ui->widgetR->graph(0)->setName("Отклонение\nорбит");
    ui->widgetR->graph(0)->setData(t, R);
    ui->widgetR->xAxis->setLabel("t, сек");
    ui->widgetR->yAxis->setLabel("R, км");
    ui->widgetR->xAxis->setRange(*std::min_element(t.begin(), t.end()), *std::max_element(t.begin(), t.end()));
    ui->widgetR->yAxis->setRange(*std::min_element(R.begin(), R.end()), *std::max_element(R.begin(), R.end()));
    ui->widgetR->replot();
}

/* on_pushButtonSD_clicked() – сохранение таблицы расчётов по ИД.
*  Локальные переменные:
*       Path – путь к файлу в заданной директории;
*       file – файл для записи таблицы расчётов по ИД.
*/

void GraphDialog::on_pushButtonSD_clicked()
{
    if(!QDir(Set->value("changed/dirSDTable").toString()).exists())
    {                                       // Условие проверки существования директории
        QMessageBox::critical
                (this, "Ошибка", "Файлы не сохранены, проверьте путь сохранения!");
    }
    else
    {
        QString Path = QString("%1/Таблица расчётов по ИД (" + ui->comboBox->currentText() + ").txt").arg(Set->value("changed/dirSDTable").toString());
        QFile file(Path);
        if (file.open(QFile::WriteOnly | QFile::Text))
        {                                   // Условие записи информации в файл
            file.write(TableSD.toUtf8());
        }
        file.close();

        QMessageBox::information(this, "Сохранение", "Таблица расчётов по ИД успешно сохранена\n\nПуть: " + Set->value("changed/dirSDTable").toString());
    }
}

/* on_pushButtonMNI_clicked() – сохранение таблицы расчётов по МНИ.
*  Локальные переменные:
*       Path – путь к файлу в заданной директории;
*       file – файл для записи таблицы расчётов по МНИ.
*/

void GraphDialog::on_pushButtonMNI_clicked()
{
    if(!QDir(Set->value("changed/dirMNITable").toString()).exists())
    {                                       // Условие проверки существования директории
        QMessageBox::critical
                (this, "Ошибка", "Файлы не сохранены, проверьте путь сохранения!");
    }
    else
    {
        QString Path = QString("%1/Таблица расчётов по МНИ (" + ui->comboBox->currentText() + ").txt").arg(Set->value("changed/dirMNITable").toString());
        QFile file(Path);
        if (file.open(QFile::WriteOnly | QFile::Text))
        {                                   // Условие записи информации в файл
            file.write(TableMNI.toUtf8());
        }
        file.close();

        QMessageBox::information(this, "Сохранение", "Таблица расчётов по МНИ успешно сохранена\n\nПуть: " + Set->value("changed/dirMNITable").toString());
    }
}

/* on_pushButtonDiverg_clicked() – сохранение таблицы отклонений орбит.
*  Локальные переменные:
*       Path – путь к файлу в заданной директории;
*       file – файл для записи таблицы отклонений орбит.
*/

void GraphDialog::on_pushButtonDiverg_clicked()
{
    if(!QDir(Set->value("changed/dirDivergTable").toString()).exists())
    {                                       // Условие проверки существования директории
        QMessageBox::critical
                (this, "Ошибка", "Файлы не сохранены, проверьте путь сохранения!");
    }
    else
    {
        QString Path = QString("%1/Таблица отклонений (" + ui->comboBox->currentText() + ").txt").arg(Set->value("changed/dirDivergTable").toString());
        QFile file(Path);
        if (file.open(QFile::WriteOnly | QFile::Text))
        {                                   // Условие записи информации в файл
            file.write(TableDiverg.toUtf8());
        }
        file.close();

        QMessageBox::information(this, "Сохранение", "Таблица отклонений успешно сохранена\n\nПуть: " + Set->value("changed/dirDivergTable").toString());
    }
}

/* on_pushButtonAllTable_clicked() – сохранение всех таблиц расчётов.
*  Локальные переменные:
*       Path – путь к файлам в заданной директории;
*       file1 - файл для записи таблицы расчётов по ИД;
*       file2 - файл для записи таблицы расчётов по МНИ;
*       file3 – файл для записи таблицы отклонений орбит.
*/

void GraphDialog::on_pushButtonAllTable_clicked()
{
    if(!QDir(Set->value("changed/dirAllTable").toString()).exists())
    {                                       // Условие проверки существования директории
        QMessageBox::critical
                (this, "Ошибка", "Файлы не сохранены, проверьте путь сохранения!");
    }
    else
    {
        QString Path = QString("%1/Таблица расчётов по ИД (" + ui->comboBox->currentText() + ").txt").arg(Set->value("changed/dirAllTable").toString());
        QFile file1(Path);
        if (file1.open(QFile::WriteOnly | QFile::Text))
        {
            file1.write(TableSD.toUtf8());
        }
        file1.close();

        Path = QString("%1/Таблица расчётов по МНИ (" + ui->comboBox->currentText() + ").txt").arg(Set->value("changed/dirAllTable").toString());
        QFile file2(Path);
        if (file2.open(QFile::WriteOnly | QFile::Text))
        {
            file2.write(TableMNI.toUtf8());
        }
        file2.close();

        Path = QString("%1/Таблица отклонений орбит (" + ui->comboBox->currentText() + ").txt").arg(Set->value("changed/dirAllTable").toString());
        QFile file3(Path);
        if (file3.open(QFile::WriteOnly | QFile::Text))
        {
            file3.write(TableDiverg.toUtf8());
        }
        file3.close();

        QMessageBox::information(this, "Сохранение", "Все таблицы расчётов успешно сохранены\n\nПуть: " + Set->value("changed/dirAllTable").toString());
    }
}

// on_pushButtonGraph_clicked() – сохранение графиков расхождений.

void GraphDialog::on_pushButtonGraph_clicked()
{
    if(!QDir(Set->value("changed/dirAllGraph").toString()).exists())
    {                                       // Условие проверки существования директории
        QMessageBox::critical
                (this, "Ошибка", "Файлы не сохранены, проверьте путь сохранения!");
    }
    else
    {
        ui->widgetX->savePng(Set->value("changed/dirAllGraph").toString() +
                             "/График расхождений по координате X (" + ui->comboBox->currentText() + ").png", 1920, 1080);
        ui->widgetY->savePng(Set->value("changed/dirAllGraph").toString() +
                             "/График расхождений по координате Y (" + ui->comboBox->currentText() + ").png", 1920, 1080);
        ui->widgetZ->savePng(Set->value("changed/dirAllGraph").toString() +
                             "/График расхождений по координате Z (" + ui->comboBox->currentText() + ").png", 1920, 1080);
        ui->widgetR->savePng(Set->value("changed/dirAllGraph").toString() +
                             "/График отклонения текущей орбиты от расчитанной теоретически (" + ui->comboBox->currentText() + ").png", 1920, 1080);
        QMessageBox::information
                (this, "Сохранение",
                 "Графики успешно сохранены\n\nПуть: " +
                 Set->value("changed/dirAllGraph").toString());
    }
}
