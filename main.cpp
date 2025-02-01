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
        for(int j = 0; j < 500; j++)
        {
            w.graphicsView->addCell(CellType::types[i], QVector2D(QRandomGenerator::global()->generate()%5000,
                                                                  QRandomGenerator::global()->generate()%5000), QVector2D(0, 0));
        }
    }
    return a.exec();
}
