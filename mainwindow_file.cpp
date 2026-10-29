#include "mainwindow.h"

#include <QFile>
#include <QIODevice>
#include <QString>
#include <QTextStream>
#include <QVector2D>

#include "celltype.h"
#include "myqgraphicsview.h"

void MainWindow::fileSelectedSlot(QString file)
{
    QFile file_(file);
    QTextStream stream(&file_);
    if(saving)
    {
        saving = false;
        file_.open(QIODeviceBase::WriteOnly);
        stream << graphicsView->cells.cells.size() << '\n';
        for(quint16 i = 0; i < graphicsView->cells.cells.size(); i++)
        {
            stream << graphicsView->cells.cells[i]->position.x() << ' ' << graphicsView->cells.cells[i]->position.y() << ' ' <<
                graphicsView->cells.cells[i]->speed.x() << ' ' << graphicsView->cells.cells[i]->speed.y() << ' ' <<
                graphicsView->cells.cells[i]->type->name << '\n';
            /*stream << graphicsView->cells.cells[i]->connectionsInd.size() << ' ';
            for(quint8 j = 0; j < graphicsView->cells.cells[i]->connectionsInd.size(); j++)
            {
                stream << graphicsView->cells.cells[i]->connectionsInd[i] << ' ';
            }
            stream << '\n' << graphicsView->cells.cells[i]->numberOfTypeConnections.size() << ' ';
            for(quint8 j = 0; j < graphicsView->cells.cells[i]->numberOfTypeConnections.size(); j++)
            {
                stream << graphicsView->cells.cells[i]->numberOfTypeConnections[i] << ' ';
            }
            stream << '\n';*/
            stream << graphicsView->cells.cells[i]->collisionAble << ' ' << graphicsView->cells.cells[i]->moveAble << '\n';
        }
        file_.close();
    }
    if(loading)
    {
        loading = false;
        file_.open(QIODeviceBase::ReadOnly);
        quint16 buff1, buff3;
        qreal buff2;
        QVector2D pos, spd;
        QString nm;
        CellType *cltp;
        stream >> buff1;
        for(int i = 0; i < buff1; i++)
        {
            stream >> buff2;
            pos.setX(buff2);
            stream >> buff2;
            pos.setY(buff2);
            stream >> buff2;
            spd.setX(buff2);
            stream >> buff2;
            spd.setY(buff2);
            stream >> nm;
            for(int j = 0; j < CellType::types.size(); j++)
            {
                if(CellType::types[j]->name == nm)
                {
                    cltp = CellType::types[j];
                    break;
                }
            }
            graphicsView->addCell(cltp, pos, spd);
            //and bounds
            stream >> buff3;
            graphicsView->cells.cells.last()->collisionAble = buff3;
            stream >> buff3;
            graphicsView->cells.cells.last()->moveAble = buff3;
        }
        file_.close();
    }
}
