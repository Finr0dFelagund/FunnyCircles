#ifndef MYQGRAPHICSVIEW_H
#define MYQGRAPHICSVIEW_H

#include <QGraphicsView>
#include "cells.h"
#include "celltype.h"
#include <QMouseEvent>
#include <QGraphicsEllipseItem>
#include <QApplication>

class Cell;

class MyQGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyQGraphicsView(QWidget *parent = Q_NULLPTR);

    QTimer* ticker;
    QTimer* drawer;

    QGraphicsScene scene;
    Cells cells;
    QVector<QGraphicsEllipseItem*> circles;
    QVector<QGraphicsLineItem*> bounds;

    void startSimulation(quint16 period = 0);
    void stopSimulation();
    void startRendering(quint8 period = 0);
    void stopRendering();

    void addCell(CellType *type, QVector2D position, QVector2D speed);
    void deleteCell(quint16 id);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    quint8 clkFlag;

public slots:
    void slotTickerAlarm();
    void slotDrawerAlarm();

signals:
    void signalInteract(quint8 button, quint8 eventx);
    void mouseInteractSignal(quint16 cell, quint8 button, quint8 eventx);
};

#endif // MYQGRAPHICSVIEW_H
