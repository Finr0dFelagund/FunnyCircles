#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QSettings>
#include <QSpinBox>
#include <QWidget>

#include "globalvars.h"

class SettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = nullptr);

    QGridLayout *layout;
    QCheckBox *bounds, *gravity, *friction, *collisions, *slowMode;
    QSpinBox *maxSpeed, *maxBoundEnergy, *g, *updateTime, *boundStiffnessFactor, *redrawTime, *cellsPerThread;
    QLabel *boundsLabel, *gravityLabel, *frictionLabel, *collisionsLabel, *maxSpeedLabel, *maxBoundEnergyLabel,
            *gLabel, *updateTimeLabel, *boundStiffnessFactorLabel, *redrawTimeLabel, *settingsLabel, *slowModeLabel,
            *cellsPerThreadLabel;
    QSettings *settings;

public slots:
    void boundsSlot(int datax);
    void gravitySlot(int datax);
    void frictionSlot(int datax);
    void collisionsSlot(int datax);
    void slowModeSlot(int data);

    void maxSpeedSlot(int datax);
    void maxBoundEnergySlot(int datax);
    void gSlot(int datax);
    void updateTimeSlot(int datax);
    void boundStiffnessFactorSlot(int datax);
    void redrawTimeSlot(int datax);
    void cellsPerThreadSlot(int datax);
};

#endif // SETTINGSWIDGET_H
