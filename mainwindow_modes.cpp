#include "mainwindow.h"

#include <QAbstractButton>
#include <QButtonGroup>
#include <QFileDialog>
#include <QPushButton>
#include <QtMath>

#include "globalvars.h"
#include "myqgraphicsview.h"

void  MainWindow::deleteCellButSlot()
{
    if(isSimulationStopped)
    {
        reset_modes();
        deleteMode = 1;
    }
}

void MainWindow::toggleMoveableButSlot()
{
    reset_modes();
    toggleMoveableMode = 1;
}

void MainWindow::toggleCollisionsButSlot()
{
    reset_modes();
    toggleCollisionsMode = 1;
}

void MainWindow::createConnectionButSlot()
{
    reset_modes();
    boundMode = 1;

    bufferCell = 0;
}

void MainWindow::removeConnectionButSlot()
{
    reset_modes();
    unBoundMode = 1;

    bufferCell = 0;
}

void MainWindow::stopButSlot()
{
    reset_modes();
    stopMode = 1;
}

void MainWindow::removeAllConnectionsButSlot()
{
    reset_modes();
    removeAllBoundsMode = 1;
}

void MainWindow::grabButSlot()
{
    reset_modes();
    grabMode = 1;
}

void MainWindow::myToolbarClicked(QAbstractButton* button)
{
    foreach(QAbstractButton* i, toolbar->buttons())
    {
        i->blockSignals(true);
        i->setChecked(i == button);
        i->blockSignals(false);
    }
}

void MainWindow::myToolbar2Clicked(QAbstractButton *button)
{
    if(button == toggleSimulationBut)
    {
        if(isSimulationStopped)
        {
            graphicsView->startSimulation();
            isSimulationStopped = 0;
            if(deleteMode)
            {
                deleteMode = 0;
            }
            if(addMode)
            {
                addMode = 0;
            }
            deleteCellBut->setEnabled(false);
            addCellBut->setEnabled(false);
            clearBut->setEnabled(false);
            saveBut->setEnabled(false);
            loadBut->setEnabled(false);
        }
        else
        {
            graphicsView->stopSimulation();
            isSimulationStopped = 1;
            deleteCellBut->setEnabled(true);
            deleteCellBut->setChecked(false);
            addCellBut->setEnabled(true);
            addCellBut->setChecked(false);
            clearBut->setEnabled(true);
            saveBut->setEnabled(true);
            loadBut->setEnabled(true);
        }
    }
    else if(button == toggleRenderBut)
    {
        if(isRenderStopped)
        {
            graphicsView->startRendering();
            isRenderStopped = 0;
        }
        else
        {
            graphicsView->stopRendering();
            isRenderStopped = 1;
        }
    }
    else if(button == loadBut)
    {
        fileDialog->setFileMode(QFileDialog::ExistingFile);
        fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
        fileDialog->open();
        loading = true;
    }
    else if(button == clearBut)
    {
        while(graphicsView->cells.cells.size())
        {
            graphicsView->deleteCell(0);
        }
    }
    else if(button == saveBut)
    {
        fileDialog->setAcceptMode(QFileDialog::AcceptSave);
        fileDialog->open();
        saving = true;
    }
}

void MainWindow::speedSliderSlot(int value)
{
    GLOBALVARS::simulationSpeed = qPow(2, value/15.0);
    graphicsView->startSimulation();
    if(isSimulationStopped)
    {
        graphicsView->stopSimulation();
    }
}

// Сброс всех переключателей режимов взаимодействия.
void MainWindow::reset_modes()
{
    addMode = 0;
    deleteMode = 0;
    boundMode = 0;
    unBoundMode = 0;
    grabMode = 0;
    stopMode = 0;
    toggleMoveableMode = 0;
    toggleCollisionsMode = 0;
    removeAllBoundsMode = 0;
}
