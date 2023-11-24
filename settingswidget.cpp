#include "settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent)
{
    settings = new QSettings("settings.ini", QSettings::IniFormat);

    settings->beginGroup("General");
    GLOBALVARS::enableBounds = settings->value("bounds", 1).toInt();
    GLOBALVARS::enableGravity = settings->value("gravity", 1).toInt();
    GLOBALVARS::enableFriction = settings->value("friction", 0).toInt();
    GLOBALVARS::enableCollisions = settings->value("collisions", 1).toInt();
    GLOBALVARS::maxSpeed = settings->value("maxSpeed", -1).toInt();
    GLOBALVARS::maxBoundEnergy = settings->value("maxBoundEnergy", 30000).toInt();
    GLOBALVARS::gravityAcceleration = settings->value("g", 10).toInt();
    GLOBALVARS::updateTime = settings->value("updateTime", 1).toInt();
    GLOBALVARS::boundStiffnessFactor = settings->value("boundStiffness", 3000).toInt();
    GLOBALVARS::redrawTime = settings->value("redrawTime", 33).toInt();

    layout = new QGridLayout;

    bounds = new QCheckBox;
    bounds->setChecked(GLOBALVARS::enableBounds);
    QWidget::connect(bounds, SIGNAL(stateChanged(int)), this, SLOT(boundsSlot(int)));

    slowMode = new QCheckBox;
    slowMode->setChecked(GLOBALVARS::enableBounds);
    QWidget::connect(slowMode, SIGNAL(stateChanged(int)), this, SLOT(slowModeSlot(int)));

    gravity = new QCheckBox;
    gravity->setChecked(GLOBALVARS::enableGravity);
    QWidget::connect(gravity, SIGNAL(stateChanged(int)), this, SLOT(gravitySlot(int)));

    friction = new QCheckBox;
    friction->setChecked(GLOBALVARS::enableFriction);
    QWidget::connect(friction, SIGNAL(stateChanged(int)), this, SLOT(frictionSlot(int)));

    collisions = new QCheckBox;
    collisions->setChecked(GLOBALVARS::enableCollisions);
    QWidget::connect(collisions, SIGNAL(stateChanged(int)), this, SLOT(collisionsSlot(int)));


    maxSpeed = new QSpinBox;
    maxSpeed->setMinimum(-1);
    maxSpeed->setMaximum(999);
    maxSpeed->setValue(GLOBALVARS::maxSpeed);
    QWidget::connect(maxSpeed, SIGNAL(valueChanged(int)), this, SLOT(maxSpeedSlot(int)));

    maxBoundEnergy = new QSpinBox;
    maxBoundEnergy->setMinimum(0);
    maxBoundEnergy->setMaximum(99999);
    maxBoundEnergy->setValue(GLOBALVARS::maxBoundEnergy);
    QWidget::connect(maxBoundEnergy, SIGNAL(valueChanged(int)), this, SLOT(maxBoundEnergySlot(int)));

    g = new QSpinBox;
    g->setMinimum(0);
    g->setMaximum(999);
    g->setValue(GLOBALVARS::gravityAcceleration);
    QWidget::connect(g, SIGNAL(valueChanged(int)), this, SLOT(gSlot(int)));

    updateTime = new QSpinBox;
    updateTime->setMinimum(1);
    updateTime->setMaximum(999);
    updateTime->setValue(GLOBALVARS::updateTime);
    QWidget::connect(updateTime, SIGNAL(valueChanged(int)), this, SLOT(updateTimeSlot(int)));

    boundStiffnessFactor = new QSpinBox;
    boundStiffnessFactor->setMinimum(0);
    boundStiffnessFactor->setMaximum(99999);
    boundStiffnessFactor->setValue(GLOBALVARS::boundStiffnessFactor);
    QWidget::connect(boundStiffnessFactor, SIGNAL(valueChanged(int)), this, SLOT(boundStiffnessFactorSlot(int)));

    redrawTime = new QSpinBox;
    redrawTime->setMinimum(1);
    redrawTime->setMaximum(999);
    redrawTime->setValue(GLOBALVARS::redrawTime);
    QWidget::connect(redrawTime, SIGNAL(valueChanged(int)), this, SLOT(redrawTimeSlot(int)));

    boundsLabel = new QLabel("Включить связи");
    gravityLabel = new QLabel("Включить поля");
    frictionLabel = new QLabel("Включить трение");
    collisionsLabel = new QLabel("Включить столкновения");
    maxSpeedLabel = new QLabel("Максимальная скорость(-1 выключено)");
    maxBoundEnergyLabel = new QLabel("Длина связей");
    gLabel = new QLabel("Ускорение свободного падения");
    updateTimeLabel = new QLabel("Длина шага симуляции, мс");
    boundStiffnessFactorLabel = new QLabel("Твёрдость связей");
    redrawTimeLabel = new QLabel("Время обновления картинки, мс");
    settingsLabel = new QLabel("Настройки:");
    slowModeLabel = new QLabel("Медленный режим");

    layout->addWidget(boundsLabel, 1, 0, 1, 1, Qt::AlignLeft);
    layout->addWidget(gravityLabel, 2, 0, 1, 1, Qt::AlignLeft);
    layout->addWidget(frictionLabel, 3, 0, 1, 1, Qt::AlignLeft);
    layout->addWidget(collisionsLabel, 4, 0, 1, 1, Qt::AlignLeft);
    layout->addWidget(maxSpeedLabel, 5, 0, 1, 1, Qt::AlignLeft);
    layout->addWidget(maxBoundEnergyLabel, 6, 0, 1, 1, Qt::AlignLeft);
    layout->addWidget(gLabel, 7, 0, 1, 1, Qt::AlignLeft);
    layout->addWidget(updateTimeLabel, 8, 0, 1, 1, Qt::AlignLeft);
    layout->addWidget(boundStiffnessFactorLabel, 9, 0, 1, 1, Qt::AlignLeft);
    layout->addWidget(redrawTimeLabel, 10, 0, 1, 1, Qt::AlignLeft);
    layout->addWidget(slowModeLabel, 11, 0, 1, 1, Qt::AlignLeft);
    layout->addWidget(settingsLabel, 0, 0, 1, 1, Qt::AlignLeft);

    layout->addWidget(bounds, 1, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(gravity, 2, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(friction, 3, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(collisions, 4, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(maxSpeed, 5, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(maxBoundEnergy, 6, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(g, 7, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(updateTime, 8, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(boundStiffnessFactor, 9, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(redrawTime, 10, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(slowMode, 11, 1, 1, 1, Qt::AlignRight);

    setLayout(layout);
    settings->endGroup();
}

void SettingsWidget::boundsSlot(int datax)
{
    settings->beginGroup("General");
    settings->setValue("bounds", datax);
    settings->endGroup();
    GLOBALVARS::enableBounds = datax;
}
void SettingsWidget::gravitySlot(int datax)
{
    settings->beginGroup("General");
    settings->setValue("gravity", datax);
    settings->endGroup();
    GLOBALVARS::enableGravity = datax;
}
void SettingsWidget::frictionSlot(int datax)
{
    settings->beginGroup("General");
    settings->setValue("friction", datax);
    settings->endGroup();
    GLOBALVARS::enableFriction = datax;
}
void SettingsWidget::collisionsSlot(int datax)
{
    settings->beginGroup("General");
    settings->setValue("collisions", datax);
    settings->endGroup();
    GLOBALVARS::enableCollisions = datax;
}

void SettingsWidget::maxSpeedSlot(int datax)
{
    settings->beginGroup("General");
    settings->setValue("maxSpeed", datax);
    settings->endGroup();
    GLOBALVARS::maxSpeed = datax;
}
void SettingsWidget::maxBoundEnergySlot(int datax)
{
    settings->beginGroup("General");
    settings->setValue("maxBoundEnergy", datax);
    settings->endGroup();
    GLOBALVARS::maxBoundEnergy = datax;
}
void SettingsWidget::gSlot(int datax)
{
    settings->beginGroup("General");
    settings->setValue("g", datax);
    settings->endGroup();
    GLOBALVARS::gravityAcceleration = datax;
}
void SettingsWidget::updateTimeSlot(int datax)
{
    settings->beginGroup("General");
    settings->setValue("updateTime", datax);
    settings->endGroup();
    GLOBALVARS::updateTime = datax;
}
void SettingsWidget::boundStiffnessFactorSlot(int datax)
{
    settings->beginGroup("General");
    settings->setValue("boundStiffness", datax);
    settings->endGroup();
    GLOBALVARS::boundStiffnessFactor = datax;
}
void SettingsWidget::redrawTimeSlot(int datax)
{
    settings->beginGroup("General");
    settings->setValue("redrawTime", datax);
    settings->endGroup();
    GLOBALVARS::redrawTime = datax;
}

void SettingsWidget::slowModeSlot(int data)
{
    if(data == 2)
    {
        GLOBALVARS::simulationSpeed = GLOBALVARS::slowModeRait;
    }
    else GLOBALVARS::simulationSpeed = 10;
}
