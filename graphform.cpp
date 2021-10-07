#include "graphform.h"
#include "ui_graphform.h"

GraphForm::GraphForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphForm)
{
    ui->setupUi(this);

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
}

GraphForm::~GraphForm()
{
    delete ui;
}
