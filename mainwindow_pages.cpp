#include "mainwindow.h"

#include <QGridLayout>
#include <QPushButton>
#include <QSlider>
#include <QWidget>

#include "addcellwidget.h"
#include "globalvars.h"
#include "myqgraphicsview.h"
#include "settingswidget.h"
#include "typecreationwidget.h"

void  MainWindow::addCellButSlot()
{
    if(isSimulationStopped)
    {
        reset_modes();
        addMode = 1;

        save_view_geometry();
        show_page(new AddCellWidget);
        connect(centralWidget(), SIGNAL(typeChosenSignal(int)), this, SLOT(cellChosenSlot(int)));
        fit_window_to_page();
    }
}

void MainWindow::createTypeButSlot()
{
    reset_modes();

    save_view_geometry();
    show_page(new TypeCreationWidget);
    QObject::connect(centralWidget(), SIGNAL(destroyed(QObject*)), this, SLOT(typeCreated()));
    fit_window_to_page();
}

void MainWindow::settingsSlot()
{
    if(isSettingsView)
    {
        isSettingsView = 0;
        takeCentralWidget();
        restore_main_page();
        if(GLOBALVARS::simulationSpeed == GLOBALVARS::slowModeRait)
        {
            speedSlider->hide();
        }
        else
        {
            speedSlider->show();
            GLOBALVARS::simulationSpeed = speedSlider->value();
        }
        resume_view();
    }
    else
    {
        isSettingsView = 1;
        save_view_geometry();
        show_page(settingsWidget);
        settingsWidget->layout->addWidget(settingsBut, 0, 1, 1, 1, Qt::AlignTop | Qt::AlignRight);
        fit_window_to_page();
    }
}

void MainWindow::typeCreated()
{
    restore_main_page();
    resume_view();
    myToolbar2Clicked(clearBut);
}

void MainWindow::cellChosenSlot(int id)
{
    bufferTypeId = (id == -1) ? bufferTypeId : id;
    restore_main_page();
    resume_view();
}

// Сохранение текущих размеров главного окна.
void MainWindow::save_view_geometry()
{
    bufferHeight = height();
    bufferWidth = width();
}

// Приостановка расчёта и отрисовки сцены.
void MainWindow::suspend_view()
{
    graphicsView->stopSimulation();
    graphicsView->stopRendering();
}

// Возобновление расчёта и отрисовки сцены, если они не остановлены пользователем.
void MainWindow::resume_view()
{
    if(isSimulationStopped == 0)
    {
        graphicsView->startSimulation();
    }
    if(isRenderStopped == 0)
    {
        graphicsView->startRendering();
    }
}

// Замена центральной страницы на служебную.
void MainWindow::show_page(QWidget* page)
{
    suspend_view();
    takeCentralWidget();
    setCentralWidget(page);
}

// Подгонка размеров окна под служебную страницу.
void MainWindow::fit_window_to_page()
{
    adjustSize();
    setMaximumSize(width(), height());
}

// Возврат к странице со сценой.
void MainWindow::restore_main_page()
{
    setMaximumSize(2048, 2048);
    setCentralWidget(mainWidget);
    mainLayout->addWidget(settingsBut, 0, 2, 1, 1, Qt::AlignTop | Qt::AlignRight);
    resize(bufferWidth, bufferHeight);
    setMinimumWidth(standartButtonToolbar2Size * 3 * 6 + 300 + 60);
}
