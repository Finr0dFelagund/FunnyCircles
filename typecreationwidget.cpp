#include "typecreationwidget.h"

TypeCreationWidget::TypeCreationWidget(QWidget *parent) : QWidget(parent),
    spinboxWidth(50), spinboxHeight(20)
{
    layout = new QGridLayout;
    setLayout(layout);

    spinboxes = new QGridLayout;

    settings = new QSettings("settings.ini", QSettings::IniFormat);
    settings->beginGroup("Types");

    sizeLab = new QLabel("Диаметр");
    massLab = new QLabel("Масса");
    FrictionLab = new QLabel("Коэффицент\nтрения");
    hardnessLab = new QLabel("Коэффицент\nжёсткости");
    spinboxes->addWidget(massLab, 0, 0, 1, 1);
    spinboxes->addWidget(sizeLab, 0, 1, 1, 1);
    spinboxes->addWidget(FrictionLab, 0, 2, 1, 1);
    spinboxes->addWidget(hardnessLab, 0, 3, 1, 1);

    mass = new QSpinBox();
    mass->setMinimum(1);
    mass->setMaximum(999999);
    mass->setFixedSize(spinboxWidth, spinboxHeight);
    spinboxes->addWidget(mass, 1, 0, 1, 1);

    size = new QSpinBox();
    size->setMinimum(1);
    size->setMaximum(9999);
    size->setFixedSize(spinboxWidth, spinboxHeight);
    spinboxes->addWidget(size, 1, 1, 1, 1);

    frictionCoeff = new QSpinBox();
    frictionCoeff->setMinimum(1);
    frictionCoeff->setMaximum(99999);
    frictionCoeff->setFixedSize(spinboxWidth, spinboxHeight);
    spinboxes->addWidget(frictionCoeff, 1, 2, 1, 1);

    hardnessFactor = new QSpinBox();
    hardnessFactor->setMinimum(1);
    hardnessFactor->setMaximum(999999);
    hardnessFactor->setFixedSize(spinboxWidth, spinboxHeight);
    spinboxes->addWidget(hardnessFactor, 1, 3, 1, 1);

    colourBut = new QPushButton(" ");
    connect(colourBut, SIGNAL(clicked(bool)), this, SLOT(colourChangedSlot()));
    colourBut->setFixedSize(spinboxHeight*2, spinboxHeight*2);
    spinboxes->addWidget(colourBut, 0, 4, 2, 1);

    createBut = new QPushButton("Создать");
    connect(createBut, SIGNAL(clicked(bool)), this, SLOT(createSlot()));

    cancelBut = new QPushButton("Отмена");
    connect(cancelBut, SIGNAL(clicked(bool)), this, SLOT(cancelSlot()));

    name = new QLineEdit();

    interactDirectBox.append(new QSpinBox);
    maxConnectionsNumberBox.append(new QSpinBox);
    maxConnectionsNumberBox.append(new QSpinBox);
    for(quint8 i = 0; i < CellType::types.count(); i++)
    {
        maxConnectionsNumberBox.append(new QSpinBox);
        maxConnectionsOtherNumberBox.append(new QSpinBox);
        interactDirectBox.append(new QSpinBox);
        interactOtherDirectBox.append(new QSpinBox);
        deleteButtons.append(new QPushButton("Удалить"));
        delBut.addButton(deleteButtons.last(), i);
    }
    QObject::connect(&delBut, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(deleteTypeSlot(QAbstractButton*)));

    for(quint8 i = 0; i < maxConnectionsNumberBox.count(); i++)
    {
        maxConnectionsNumberBox[i]->setMinimum(-1);
        maxConnectionsNumberBox[i]->setMaximum(99);
        maxConnectionsNumberBox[i]->setFixedWidth(spinboxWidth);
    }
    for(quint8 i = 0; i < maxConnectionsOtherNumberBox.count(); i++)
    {
        maxConnectionsOtherNumberBox[i]->setMinimum(-1);
        maxConnectionsOtherNumberBox[i]->setMaximum(99);
        maxConnectionsOtherNumberBox[i]->setFixedWidth(spinboxWidth);
    }
    for(quint8 i = 0; i < interactDirectBox.count(); i++)
    {
        interactDirectBox[i]->setMinimum(-1);
        interactDirectBox[i]->setMaximum(1);
        interactDirectBox[i]->setFixedWidth(spinboxWidth);
    }
    for(quint8 i = 0; i < interactOtherDirectBox.count(); i++)
    {
        interactOtherDirectBox[i]->setMinimum(-1);
        interactOtherDirectBox[i]->setMaximum(1);
        interactOtherDirectBox[i]->setFixedWidth(spinboxWidth);
    }
    for(quint8 i = 0; i < deleteButtons.count(); i++)
    {
        deleteButtons[i]->setFixedWidth(spinboxWidth*1.2);
    }

    nameLabel = new QLabel("Имя");
    nameLabel->setFixedWidth(spinboxWidth*5);
    layout->addWidget(nameLabel, 0, 0, 1, 1);

    layout->addWidget(name, 0, 1, 1, 6);
    layout->addLayout(spinboxes, 1, 0, 1, 4);

    for(quint8 i = 0; i < CellType::types.count(); i++)
    {
        typeNames.append(new QLabel(CellType::types[i]->name));
        layout->addWidget(typeNames.last(), i+3, 0, 1, 1);
    }
    typeNames.append(new QLabel("Данный"));
    all = new QLabel("Всего");
    all->setFixedWidth(spinboxWidth*5);

    layout->addWidget(typeNames.last(), typeNames.count() + 2, 0, 1, 1);
    layout->addWidget(all, typeNames.count() + 3, 0, 1, 1);

    for(quint8 i = 0; i < maxConnectionsNumberBox.count(); i++)
    {
        layout->addWidget(maxConnectionsNumberBox[i], i+3, 3, 1, 1);
    }
    for(quint8 i = 0; i < maxConnectionsOtherNumberBox.count(); i++)
    {
        layout->addWidget(maxConnectionsOtherNumberBox[i], i+3, 4, 1, 1);
    }
    for(quint8 i = 0; i < interactDirectBox.count(); i++)
    {
        layout->addWidget(interactDirectBox[i], i+3, 1, 1, 1);
    }
    for(quint8 i = 0; i < interactOtherDirectBox.count(); i++)
    {
        layout->addWidget(interactOtherDirectBox[i], i+3, 2, 1, 1);
    }
    for(quint8 i = 0; i < deleteButtons.count(); i++)
    {
        layout->addWidget(deleteButtons[i], i+3, 5, 1, 1);
    }

    label1 = new QLabel("Имя типа");
    label2 = new QLabel("Сила на тип");
    label3 = new QLabel("Сила от типа");
    label4 = new QLabel("Связей с типом");
    label5 = new QLabel("Связей типа");
    label1->setFixedWidth(spinboxWidth*5);
    label2->setFixedWidth(spinboxWidth*5);
    label3->setFixedWidth(spinboxWidth*5);
    label4->setFixedWidth(spinboxWidth*5);
    label5->setFixedWidth(spinboxWidth*5);
    layout->addWidget(label1, 2, 0, 1, 1);
    layout->addWidget(label2, 2, 1, 1, 1);
    layout->addWidget(label3, 2, 2, 1, 1);
    layout->addWidget(label4, 2, 3, 1, 1);
    layout->addWidget(label5, 2, 4, 1, 1);


    info = new QLabel("Силы:  -1 - притягивает,\n              1 - отталкивает,\n              0 - не взаимодействует\nСвязи: -1 - бесконечность,\n              0 - не образовывает");
    layout->addWidget(info, 1, 6, 3, 2);

    layout->addWidget(createBut, typeNames.count() + 4, 4, 1, 1);
    layout->addWidget(cancelBut, typeNames.count() + 4, 5, 1, 1);
    settings->endGroup();
}

void TypeCreationWidget::colourChangedSlot()
{
    color = QColorDialog::getColor();
    if(!color.isValid())
    {
        color = QColor(Qt::GlobalColor::black);
    }
    buffPalette = colourBut->palette();
    buffPalette.setColor(QPalette::Button, color);
    colourBut->setStyleSheet("QPushButton{background: transparent;}");
    colourBut->setPalette(buffPalette);
    colourBut->setAutoFillBackground(true);
}

void TypeCreationWidget::createSlot()
{
    if(name->text().size() != 0 && color.isValid())
    {
        for(quint8 i = 0; i < maxConnectionsNumberBox.count(); i++)
        {
            maxConnectionsNumber.append(maxConnectionsNumberBox[i]->value());
        }
        for(quint8 i = 0; i < maxConnectionsOtherNumberBox.count(); i++)
        {
            maxConnectionsOtherNumber.append(maxConnectionsOtherNumberBox[i]->value());
        }
        for(quint8 i = 0; i < interactDirectBox.count(); i++)
        {
            interactDirect.append(interactDirectBox[i]->value());
        }
        for(quint8 i = 0; i < interactOtherDirectBox.count(); i++)
        {
            interactOtherDirect.append(interactOtherDirectBox[i]->value());
        }
        CellType::newType(QBrush(color), &GLOBALVARS::force, interactDirect, size->value(), mass->value(), frictionCoeff->value()/100.0, hardnessFactor->value(), maxConnectionsNumber,
                          interactOtherDirect, maxConnectionsOtherNumber, name->text());
        delete this;
    }
}

void TypeCreationWidget::cancelSlot()
{
    delete this;
}

void TypeCreationWidget::deleteTypeSlot(QAbstractButton *id)
{
    CellType::removeType(CellType::types[delBut.id(id)]);
    delete this;
}

TypeCreationWidget::~TypeCreationWidget()
{
    settings->beginGroup("Types");
    settings->setValue("number", CellType::types.count());
    settings->beginWriteArray("types");
    for(quint8 i = 0; i < CellType::types.count(); i++)
    {
        settings->setArrayIndex(i);
        settings->setValue("number", i);
        settings->setValue("name", CellType::types[i]->name);
        int r, g, b;
        CellType::types[i]->brush.color().getRgb(&r, &g, &b);
        settings->setValue("color1", r);
        settings->setValue("color2", g);
        settings->setValue("color3", b);
        settings->setValue("size", CellType::types[i]->size);
        settings->setValue("mass", CellType::types[i]->mass);
        settings->setValue("friction", CellType::types[i]->frictionCoeff);
        settings->setValue("hardness", CellType::types[i]->hardnessFactor);
        QList<QVariant> buff;
        for(quint8 j = 0; j < CellType::types[i]->interactDirection.count(); j++)
        {
            buff.append(CellType::types[i]->interactDirection[j]);
        }
        settings->setValue("interactDirection", buff);
        buff.clear();
        for(quint8 j = 0; j < CellType::types[i]->maxConnectionsNumber.count(); j++)
        {
            buff.append(CellType::types[i]->maxConnectionsNumber[j]);
        }
        settings->setValue("maxConnectionNumber", buff);
        buff.clear();
    }
    settings->endArray();
    settings->endGroup();

    delete settings;
    delete mass;
    delete size;
    delete frictionCoeff;
    delete hardnessFactor;
    delete colourBut;
    delete createBut;
    delete cancelBut;
    delete name;
    delete spinboxes;
    delete sizeLab;
    delete massLab;
    delete FrictionLab;
    delete hardnessLab;
    delete all;
    delete label1;
    delete label2;
    delete label3;
    delete label4;
    delete label5;
    delete info;
    delete nameLabel;

    for(quint8 i = 0; i < maxConnectionsNumberBox.count(); i++)
    {
        delete maxConnectionsNumberBox[i];
    }
    for(quint8 i = 0; i < maxConnectionsOtherNumberBox.count(); i++)
    {
        delete maxConnectionsOtherNumberBox[i];
    }
    for(quint8 i = 0; i < interactDirectBox.count(); i++)
    {
        delete interactDirectBox[i];
    }
    for(quint8 i = 0; i < interactOtherDirectBox.count(); i++)
    {
        delete interactOtherDirectBox[i];
    }
    for(quint8 i = 0; i < typeNames.count(); i++)
    {
        delete typeNames[i];
    }
    for(quint8 i = 0; i < deleteButtons.count(); i++)
    {
        delete deleteButtons[i];
    }
}
