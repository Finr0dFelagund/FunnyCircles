#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QButtonGroup>
#include <QFileDialog>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QPoint>
#include <QPushButton>
#include <QSlider>
#include <QTextStream>
#include <QTimer>
#include <QWidget>
#include <QtMath>

#include "addcellwidget.h"
#include "cells.h"
#include "myqgraphicsview.h"
#include "settingswidget.h"
#include "typecreationwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    MyQGraphicsView* graphicsView;
    QWidget *aboveGraphicsViewWidget;
    QGridLayout *mainLayout;
    QWidget *mainWidget;
    QGridLayout *toolbarLayout;
    QButtonGroup *toolbar;
    QWidget *toolbarWidget;
    QPushButton *addCellBut, *deleteCellBut, *toggleMoveableBut, *toggleCollisionsBut, *createConnectionBut, *removeConnectionBut, *stopBut, *removeAllConnectionsBut, *grabBut, *createTypeBut;
    QWidget *toolbar2Widget;
    QHBoxLayout *toolbar2Layout;
    QButtonGroup *toolbar2;
    QPushButton *toggleRenderBut, *toggleSimulationBut, *loadBut, *clearBut, *saveBut;

    quint8 standartButtonToolbarSize, standartButtonToolbar2Size;
    QPushButton* dividingToolbar2But;
    bool addMode, deleteMode, boundMode, unBoundMode, grabMode, stopMode, toggleMoveableMode, toggleCollisionsMode, removeAllBoundsMode;

    QSlider* speedSlider;
    QGraphicsRectItem* selectionZone;

    QPushButton *settingsBut;
    SettingsWidget *settingsWidget;
    bool isSettingsView;
    quint16 bufferHeight, bufferWidth;

    static MainWindow* myWindow;
    quint16 bufferCell;
    bool isSimulationStopped, isRenderStopped;
    QPoint bufferPosition;
    QVector2D bufferPosition2;
    bool bufferMoveAble;
    quint8 bufferTypeId;
    QGraphicsLineItem *speedLine;

    QFileDialog *fileDialog;
    bool saving, loading;

    QTimer* grabber, *selector, *speeder;

    void InitializeTypes();

public slots:
    void addCellButSlot();
    void deleteCellButSlot();
    void toggleMoveableButSlot();
    void toggleCollisionsButSlot();
    void createConnectionButSlot();
    void removeConnectionButSlot();
    void stopButSlot();
    void removeAllConnectionsButSlot();
    void grabButSlot();
    void createTypeButSlot();
    void cellInteracted(quint16 cell, quint8 mouseButton, quint8 event);
    void myToolbarClicked(QAbstractButton* button);
    void slotGrabberAlarm();
    void myToolbar2Clicked(QAbstractButton* button);
    void speedSliderSlot(int value);
    void sceneInteracted(quint8 button, quint8 event);
    void slotSelectorAlarm();
    void settingsSlot();
    void typeCreated();
    void cellChosenSlot(int id);
    void speedSettingSlot();
    void fileSelectedSlot(QString file);
};

#endif // MAINWINDOW_H

