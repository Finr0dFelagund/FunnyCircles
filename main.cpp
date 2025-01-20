#include "mainwindow.h"
#include "celltype.h"
#include <QVector2D>
#include <QApplication>
#include <QtMath>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 1000; j++)
        {
            w.graphicsView->addCell(CellType::types[i], QVector2D(QRandomGenerator::global()->generate()%1000, QRandomGenerator::global()->generate()%1000), QVector2D(0, 0));
        }
    }
    /*w.graphicsView->addCell(CellType::types[4], QVector2D(500, 500), QVector2D(0, 0));
    for(int i = 0; i < 30; i++)
    {
        w.graphicsView->addCell(CellType::types[0], QVector2D(500+qCos(3.14159*2/30*i)*300, 500+qSin(3.14159*2/30*i)*300), QVector2D(0, 0));
    }
    w.graphicsView->addCell(CellType::types[4], QVector2D(1200, 500), QVector2D(0, 0));
    for(int i = 0; i < 30; i++)
    {
        w.graphicsView->addCell(CellType::types[0], QVector2D(1200+qCos(3.14159*2/30*i)*300, 500+qSin(3.14159*2/30*i)*300), QVector2D(0, 0));
    }*/
    return a.exec();
}
