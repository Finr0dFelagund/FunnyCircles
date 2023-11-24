#include "addcellwidget.h"

AddCellWidget::AddCellWidget(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout;
    setLayout(layout);
    for(quint8 i = 0; i < CellType::types.count(); i++)
    {
        buttons.append(new QPushButton(CellType::types[i]->name));
        butGroup.addButton(buttons[i], i);
        layout->addWidget(buttons[i]);
    }
    QObject::connect(&butGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(typeChosenSlot(QAbstractButton*)));

    cancelBut = new QPushButton("Отмена");
    layout->addWidget(cancelBut);
    QObject::connect(cancelBut, SIGNAL(clicked(bool)), this, SLOT(deleteSlot()));
}

void AddCellWidget::typeChosenSlot(QAbstractButton *button)
{
    emit typeChosenSignal(butGroup.id(button));
    delete this;
}

void AddCellWidget::deleteSlot()
{
    delete this;
}

AddCellWidget::~AddCellWidget()
{
    for(quint8 i = 0; i < buttons.count(); i++)
    {
        delete buttons[i];
    }
    delete cancelBut;
    delete layout;
    emit typeChosenSignal(-1);
}
