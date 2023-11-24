#include "myqgraphicsview.h"
#include <QTimer>
#include "globalvars.h"

MyQGraphicsView::MyQGraphicsView(QWidget *parent)
    :QGraphicsView(parent), clkFlag(0)
{
    ticker = new QTimer();
    connect(ticker, SIGNAL(timeout()), this, SLOT(slotTickerAlarm()));

    drawer = new QTimer();
    connect(drawer, SIGNAL(timeout()), this, SLOT(slotDrawerAlarm()));

    setScene(&scene);
    setSceneRect(scene.sceneRect());
}

void MyQGraphicsView::slotTickerAlarm()
{
    cells.onTick(1);
}

void MyQGraphicsView::slotDrawerAlarm()
{
    for(quint16 i = 0; i < circles.size(); i++)
    {
        circles[i]->setPos(cells.cells[i]->position.x() - cells.cells[i]->type->size/2, cells.cells[i]->position.y() - cells.cells[i]->type->size/2);
    }
    for(quint16 i = 0; i < cells.connectionsInd[0].size() - bounds.size(); i++)
    {
        bounds.append(new QGraphicsLineItem(0, 0, 0, 0));
        scene.addItem(bounds.last());
    }
    while(bounds.size() > cells.connectionsInd[0].size())
    {
        delete bounds.last();
        bounds.removeLast();
    }
    Cell *a, *b;
    QVector2D atob;
    for(quint16 i = 0; i < bounds.size(); i++)
    {
        a = cells.cells[cells.connectionsInd[0][i]];
        b = cells.cells[cells.connectionsInd[1][i]];
        atob = (b->position - a->position).normalized();
        bounds[i]->setLine(a->position.x() + (atob * a->type->size/2).x(), a->position.y() + (atob * a->type->size/2).y(),
                           b->position.x() - (atob * b->type->size/2).x(), b->position.y() - (atob * b->type->size/2).y());
    }
}

void MyQGraphicsView::startSimulation(quint16 period)
{
    ticker->start(period == 0 ? GLOBALVARS::simulationSpeed : period);
}

void MyQGraphicsView::stopSimulation()
{
    ticker->stop();
}

void MyQGraphicsView::startRendering(quint8 period)
{
    drawer->start(period == 0 ? GLOBALVARS::redrawTime : period);
}

void MyQGraphicsView::stopRendering()
{
    drawer->stop();
}

void MyQGraphicsView::addCell(CellType *type, QVector2D position, QVector2D speed)
{
    cells.addCell(position, speed, type);
    circles.append(new QGraphicsEllipseItem(0, 0, type->size, type->size));
    circles.last()->setPos(position.x() - cells.cells.last()->type->size/2, position.y() - cells.cells.last()->type->size/2);
    circles.last()->setBrush(type->brush);
    scene.addItem(circles.last());
}

void MyQGraphicsView::deleteCell(quint16 id)
{
    cells.deleteCell(id);
    delete circles[id];
    circles.remove(id);
}

void MyQGraphicsView::mousePressEvent(QMouseEvent *event)
{
    quint8 button;
    if(event->button() == Qt::LeftButton)
    {
        button = 0;
    }
    if(event->button() == Qt::RightButton)
    {
        button = 1;
    }
    if(QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) && button == 0)
    {
        clkFlag = 1;
        emit signalInteract(button, 0);
    }
    else if(scene.itemAt(mapToScene(event->pos()), QTransform()) && scene.itemAt(mapToScene(event->pos()), QTransform())->data(0).toString() == "cell")
    {
        //emit mouseInteractSignal((scene.itemAt(mapToScene(event->pos()), QTransform())), button, 0);
    }
}

void MyQGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    quint8 button;
    if(event->button() == Qt::LeftButton)
    {
        button = 0;
    }
    if(event->button() == Qt::RightButton)
    {
        button = 1;
    }
    if(clkFlag)
    {
        emit signalInteract(button, 1);
        clkFlag = 0;
    }
    else if(scene.itemAt(mapToScene(event->pos()), QTransform()) && scene.itemAt(mapToScene(event->pos()), QTransform())->data(0).toString() == "cell")
    {
        //emit mouseInteractSignal(static_cast<Cell*>(scene.itemAt(mapToScene(event->pos()), QTransform())), button, 1);
    }
}

