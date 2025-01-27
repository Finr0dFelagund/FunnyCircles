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
        for(int j = 0; j < 100; j++)
        {
            w.graphicsView->addCell(CellType::types[i], QVector2D(QRandomGenerator::global()->generate()%1500, QRandomGenerator::global()->generate()%1500), QVector2D(0, 0));
        }
    }
    return a.exec();
}
