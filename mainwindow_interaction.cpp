#include "mainwindow.h"

#include <QApplication>
#include <QCursor>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QTimer>
#include <QVector2D>

#include "celltype.h"
#include "myqgraphicsview.h"

void MainWindow::cellInteracted(quint16 cell, quint8 mouseButton, quint8 event)
{
    if(mouseButton == 0)
    {
        if(event == 0)
        {
            if(deleteMode && QApplication::keyboardModifiers() != Qt::ShiftModifier)
            {
                graphicsView->deleteCell(cell);
            }
            else if(stopMode || toggleMoveableMode || toggleCollisionsMode || removeAllBoundsMode)
            {
                apply_common_cell_action(cell);
            }
            else if(boundMode)
            {
                if(bufferCell == 0)
                {
                    bufferCell = cell;
                }
                else
                {
                    graphicsView->cells.tryToConnect(bufferCell, cell);
                    bufferCell = 0;
                }
            }
            else if(unBoundMode)
            {
                if(bufferCell == 0)
                {
                    bufferCell = cell;
                }
                else
                {
                    graphicsView->cells.removeConnect(bufferCell, cell);
                    bufferCell = 0;
                }
            }
            else if(grabMode)
            {
                bufferCell = cell;
                bufferPosition = QCursor::pos();
                bufferPosition2 = graphicsView->cells.cells[cell]->position;
                bufferMoveAble = graphicsView->cells.cells[cell]->moveAble;
                graphicsView->cells.cells[cell]->moveAble = 0;
                grabber->start(5);
            }
        }
        else if(event == 1)
        {
            if(grabMode)
            {
                grabber->stop();
                graphicsView->cells.cells[cell]->moveAble = bufferMoveAble;
                graphicsView->cells.cells[cell]->speed = QVector2D(0, 0);
            }
        }
    }
}

void MainWindow::slotGrabberAlarm()
{
    QVector2D newPos = bufferPosition2;
    newPos.setX(newPos.x() + (QCursor::pos().x() - bufferPosition.x()));
    newPos.setY(newPos.y() + (QCursor::pos().y() - bufferPosition.y()));
    graphicsView->cells.cells[bufferCell]->position = newPos;
}

void MainWindow::sceneInteracted(quint8 button, quint8 event)
{
    if(button == 0)
    {
        if(event == 0)
        {
            if(!addMode)
            {
                const QPointF zone_origin = scene_position_from_cursor();
                selectionZone = new QGraphicsRectItem(QRectF(zone_origin, zone_origin + QPointF(1, 1)));
                selectionZone->setBrush(Qt::Dense7Pattern);
                graphicsView->scene.addItem(selectionZone);
                bufferPosition = QCursor::pos();
                selector->start(10);
            }
            else
            {
                const QPointF scene_pointer = scene_position_from_cursor();
                bufferPosition.setX(scene_pointer.x());
                bufferPosition.setY(scene_pointer.y());
                graphicsView->addCell(CellType::types[bufferTypeId], QVector2D(bufferPosition.x(), bufferPosition.y()), QVector2D(0, 0));
                graphicsView->cells.cells.last()->moveAble = false;
                graphicsView->cells.cells.last()->collisionAble = false;
                speedLine = new QGraphicsLineItem(bufferPosition.x(), bufferPosition.y(), bufferPosition.x()+1, bufferPosition.y()+1);
                graphicsView->scene.addItem(speedLine);
                speeder->start(10);
            }
        }
        else if(event == 1)
        {
            if(selectionZone != 0)
            {
                foreach(QGraphicsItem* item, selectionZone->collidingItems(Qt::IntersectsItemShape))
                {
                    if(item->data(0).toString() == "cell")
                    {
                        if(deleteMode)
                        {
                            graphicsView->deleteCell(graphicsView->circles.indexOf(item));
                        }
                        else if(stopMode || toggleMoveableMode || toggleCollisionsMode || removeAllBoundsMode)
                        {
                            apply_common_cell_action(graphicsView->circles.indexOf(item));
                        }
                    }
                }
                selector->stop();
                delete selectionZone;
                selectionZone = 0;
            }

            if(speedLine != 0)
            {
                speeder->stop();
                delete speedLine;
                speedLine = 0;
                graphicsView->cells.cells.last()->moveAble = true;
                graphicsView->cells.cells.last()->collisionAble = true;
                const QPointF scene_pointer = scene_position_from_cursor();
                graphicsView->cells.cells.last()->speed = QVector2D(scene_pointer.x() - bufferPosition.x(), scene_pointer.y() - bufferPosition.y());
                bufferCell = 0;
            }
        }
    }
    else if(button == 1)
    {

    }
}

void MainWindow::slotSelectorAlarm()
{
    const QPointF cursor_position = scene_position_from_cursor();
    const QPointF anchor_position = scene_position_from_global(bufferPosition);
    if(QCursor::pos().x() >= bufferPosition.x() && QCursor::pos().y() >= bufferPosition.y())
    {
        selectionZone->setRect(QRectF(anchor_position, cursor_position));
    }
    else if(QCursor::pos().x() >= bufferPosition.x() && QCursor::pos().y() < bufferPosition.y())
    {
        selectionZone->setRect(QRectF(QPointF(anchor_position.x(), cursor_position.y()), QPointF(cursor_position.x(), anchor_position.y())));
    }
    else if(QCursor::pos().x() < bufferPosition.x() && QCursor::pos().y() >= bufferPosition.y())
    {
        selectionZone->setRect(QRectF(QPointF(cursor_position.x(), anchor_position.y()), QPointF(anchor_position.x(), cursor_position.y())));
    }
    else if(QCursor::pos().x() < bufferPosition.x() && QCursor::pos().y() < bufferPosition.y())
    {
        selectionZone->setRect(QRectF(cursor_position, anchor_position));
    }
}

void MainWindow::speedSettingSlot()
{
    const QPointF pointer_position = scene_position_from_cursor();
    speedLine->setLine(bufferPosition.x(), bufferPosition.y(), pointer_position.x(), pointer_position.y());
}

// Перевод позиции курсора в координаты сцены.
QPointF MainWindow::scene_position_from_cursor() const
{
    return scene_position_from_global(QCursor::pos());
}

// Перевод точки в глобальных экранных координатах в координаты сцены.
QPointF MainWindow::scene_position_from_global(const QPoint& point) const
{
    return graphicsView->mapToScene(graphicsView->mapFromGlobal(point));
}

// Применение общих действий режимов к одной клетке.
void MainWindow::apply_common_cell_action(quint16 cell_index)
{
    if(stopMode)
    {
        graphicsView->cells.cells[cell_index]->speed = QVector2D(0, 0);
    }
    else if(toggleMoveableMode)
    {
        graphicsView->cells.cells[cell_index]->moveAble = !graphicsView->cells.cells[cell_index]->moveAble;
    }
    else if(toggleCollisionsMode)
    {
        graphicsView->cells.cells[cell_index]->collisionAble = !graphicsView->cells.cells[cell_index]->collisionAble;
    }
    else if(removeAllBoundsMode)
    {
        graphicsView->cells.removeAllConnections(cell_index);
    }
}
