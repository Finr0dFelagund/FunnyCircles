#ifndef TYPECREATIONWIDGET_H
#define TYPECREATIONWIDGET_H

#include <QButtonGroup>
#include <QColor>
#include <QColorDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPalette>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QVector>
#include <QWidget>

#include "celltype.h"
#include "globalvars.h"

class TypeCreationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TypeCreationWidget(QWidget *parent = nullptr);
    ~TypeCreationWidget();

    QSettings *settings;
    QGridLayout *layout;
    QGridLayout *spinboxes;
    QSpinBox *mass, *size, *frictionCoeff, *hardnessFactor;
    QPushButton *colourBut, *createBut, *cancelBut;
    QColor color;
    QLineEdit *name;
    QLabel *sizeLab, *massLab, *FrictionLab, *hardnessLab, *all;
    QPalette buffPalette;
    QLabel *label1, *label2, *label3, *label4, *label5, *info, *nameLabel;

    QVector<QSpinBox*> maxConnectionsNumberBox, maxConnectionsOtherNumberBox, interactDirectBox, interactOtherDirectBox;
    QVector<QPushButton*> deleteButtons;
    QButtonGroup delBut;
    QVector<QLabel*> typeNames;
    QVector<qint8> maxConnectionsNumber, maxConnectionsOtherNumber, interactDirect, interactOtherDirect;

    quint8 spinboxWidth, spinboxHeight;
signals:

public slots:
    void colourChangedSlot();
    void createSlot();
    void cancelSlot();
    void deleteTypeSlot(QAbstractButton* id);
};

#endif // TYPECREATIONWIDGET_H
