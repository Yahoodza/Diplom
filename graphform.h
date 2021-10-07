#ifndef GRAPHFORM_H
#define GRAPHFORM_H

#include <QWidget>

namespace Ui {
class GraphForm;
}

class GraphForm : public QWidget
{
    Q_OBJECT

public:
    explicit GraphForm(QWidget *parent = nullptr);
    ~GraphForm();

private:
    Ui::GraphForm *ui;
};

#endif // GRAPHFORM_H
