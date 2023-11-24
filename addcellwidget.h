#ifndef ADDCELLWIDGET_H
#define ADDCELLWIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include "celltype.h"

class AddCellWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddCellWidget(QWidget *parent = nullptr);
    ~AddCellWidget();

    QVector<QPushButton*> buttons;
    QButtonGroup butGroup;
    QVBoxLayout *layout;
    QPushButton *cancelBut;
signals:
    void typeChosenSignal(int button);
public slots:
    void typeChosenSlot(QAbstractButton* button);
    void deleteSlot();
};

#endif // ADDCELLWIDGET_H
