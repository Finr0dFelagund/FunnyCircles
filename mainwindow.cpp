#include "mainwindow.h"

#include <QAbstractButton>
#include <QButtonGroup>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSizePolicy>
#include <QSlider>
#include <QTimer>
#include <QWidget>

#include "globalvars.h"
#include "myqgraphicsview.h"
#include "settingswidget.h"

MainWindow* MainWindow::myWindow;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), standartButtonToolbarSize(40), standartButtonToolbar2Size(30),
    addMode(0), deleteMode(0), boundMode(0), unBoundMode(0), grabMode(0), stopMode(0), toggleMoveableMode(0), toggleCollisionsMode(0), removeAllBoundsMode(0),
    selectionZone(0), isSettingsView(0), bufferCell(0), isSimulationStopped(1), isRenderStopped(0), speedLine(0), saving(0), loading(0)
{
    create_timers();
    create_view();
    create_layout();
    create_speed_slider();
    create_toolbar();
    create_toolbar2();
    create_settings_widget();
    create_file_dialog();

    resize(1200, 900);
    setCentralWidget(mainWidget);
    mainWidget->setLayout(mainLayout);
    mainLayout->addWidget(toolbarWidget, 0, 0, 2, 1, Qt::AlignTop | Qt::AlignLeft);
    mainLayout->addWidget(graphicsView, 1, 1, 1, 1);
    mainLayout->addWidget(aboveGraphicsViewWidget, 0, 1, 1, 1);
    mainLayout->addWidget(toolbar2Widget, 3, 0, 1, 2, Qt::AlignBottom | Qt::AlignLeft);
    mainLayout->addWidget(settingsBut, 0, 3, 1, 1, Qt::AlignTop | Qt::AlignRight);
    setMinimumWidth(standartButtonToolbar2Size * 3 * 6 + 300 + 60);

    InitializeTypes();
}

// Создание таймеров перетаскивания, выделения и задания скорости.
void MainWindow::create_timers()
{
    grabber = new QTimer;
    connect(grabber, SIGNAL(timeout()), this, SLOT(slotGrabberAlarm()));

    selector = new QTimer;
    connect(selector, SIGNAL(timeout()), this, SLOT(slotSelectorAlarm()));

    speeder = new QTimer;
    connect(speeder, SIGNAL(timeout()), this, SLOT(speedSettingSlot()));
}

// Создание представления сцены и подключение его сигналов.
void MainWindow::create_view()
{
    myWindow = this;
    graphicsView = new MyQGraphicsView;
    connect(graphicsView, SIGNAL(signalInteract(quint8,quint8)), this, SLOT(sceneInteracted(quint8,quint8)));
    connect(graphicsView, SIGNAL(mouseInteractSignal(quint16,quint8,quint8)), this, SLOT(cellInteracted(quint16,quint8,quint8)));
    graphicsView->startRendering();
}

// Создание главного компоновщика и вспомогательных виджетов страницы.
void MainWindow::create_layout()
{
    mainLayout = new QGridLayout;
    mainWidget = new QWidget;

    aboveGraphicsViewWidget = new QWidget;
    aboveGraphicsViewWidget->setFixedSize(1, 30);
    aboveGraphicsViewWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
}

// Создание ползунка скорости симуляции.
void MainWindow::create_speed_slider()
{
    speedSlider = new QSlider(Qt::Horizontal);
    mainLayout->addWidget(speedSlider, 3, 1, 1, 2, Qt::AlignRight | Qt::AlignBottom);
    connect(speedSlider, SIGNAL(valueChanged(int)), this, SLOT(speedSliderSlot(int)));
    speedSlider->setMaximum(100);
    speedSlider->setMinimum(0);
    speedSlider->setSingleStep(1);
    speedSlider->setInvertedAppearance(1);
    speedSlider->setValue(50);
    speedSlider->setFixedSize(300, 30);
}

// Создание панели инструментов редактирования сцены.
void MainWindow::create_toolbar()
{
    toolbarLayout = new QGridLayout;
    toolbarLayout->setVerticalSpacing(0);
    toolbarLayout->setHorizontalSpacing(0);
    toolbarLayout->setContentsMargins(0, 0, 0 ,0);

    toolbarWidget = new QWidget;
    toolbarWidget->setLayout(toolbarLayout);
    toolbarWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    addCellBut = new QPushButton("add");
    connect(addCellBut, SIGNAL(clicked(bool)), this, SLOT(addCellButSlot()));
    deleteCellBut = new QPushButton("del");
    connect(deleteCellBut, SIGNAL(clicked(bool)), this, SLOT(deleteCellButSlot()));
    toggleMoveableBut = new QPushButton("mov");
    connect(toggleMoveableBut, SIGNAL(clicked(bool)), this, SLOT(toggleMoveableButSlot()));
    toggleCollisionsBut = new QPushButton("col");
    connect(toggleCollisionsBut, SIGNAL(clicked(bool)), this, SLOT(toggleCollisionsButSlot()));
    createConnectionBut = new QPushButton("con");
    connect(createConnectionBut, SIGNAL(clicked(bool)), this, SLOT(createConnectionButSlot()));
    removeConnectionBut = new QPushButton("unCon");
    connect(removeConnectionBut, SIGNAL(clicked(bool)), this, SLOT(removeConnectionButSlot()));
    stopBut = new QPushButton("stp");
    connect(stopBut, SIGNAL(clicked(bool)), this, SLOT(stopButSlot()));
    removeAllConnectionsBut = new QPushButton("rmAlB");
    connect(removeAllConnectionsBut, SIGNAL(clicked(bool)), this, SLOT(removeAllConnectionsButSlot()));
    grabBut = new QPushButton("grb");
    connect(grabBut, SIGNAL(clicked(bool)), this, SLOT(grabButSlot()));
    createTypeBut = new QPushButton("new type");
    connect(createTypeBut, SIGNAL(clicked(bool)), this, SLOT(createTypeButSlot()));

    toolbar = new QButtonGroup;
    toolbar->addButton(addCellBut, 0);
    toolbar->addButton(deleteCellBut, 1);
    toolbar->addButton(toggleMoveableBut, 2);
    toolbar->addButton(toggleCollisionsBut, 3);
    toolbar->addButton(createConnectionBut, 4);
    toolbar->addButton(removeConnectionBut, 5);
    toolbar->addButton(stopBut, 6);
    toolbar->addButton(removeAllConnectionsBut, 7);
    toolbar->addButton(grabBut, 8);
    toolbar->addButton(createTypeBut, 9);
    toolbar->setExclusive(false);

    foreach(QAbstractButton* but, toolbar->buttons())
    {
        but->setFixedSize(standartButtonToolbarSize, standartButtonToolbarSize);
        but->setCheckable(true);
    }
    createTypeBut->setFixedSize(2 * standartButtonToolbarSize, standartButtonToolbarSize);

    connect(toolbar, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(myToolbarClicked(QAbstractButton*)));

    toolbarLayout->addWidget(addCellBut, 0, 0);
    toolbarLayout->addWidget(deleteCellBut, 0, 1);
    toolbarLayout->addWidget(toggleMoveableBut, 1, 0);
    toolbarLayout->addWidget(toggleCollisionsBut, 1, 1);
    toolbarLayout->addWidget(createConnectionBut, 2, 0);
    toolbarLayout->addWidget(removeConnectionBut, 2, 1);
    toolbarLayout->addWidget(stopBut, 3, 0);
    toolbarLayout->addWidget(removeAllConnectionsBut, 3, 1);
    toolbarLayout->addWidget(grabBut, 4, 0);
    toolbarLayout->addWidget(createTypeBut, 5, 0, 1, 2);
}

// Создание панели управления симуляцией и файлами сцены.
void MainWindow::create_toolbar2()
{
    toolbar2Layout = new QHBoxLayout;
    toolbar2Layout->setSpacing(0);
    toolbar2Layout->setContentsMargins(0, 0, 0, 0);
    toolbar2Layout->setContentsMargins(0, 0, 0 ,0);

    toolbar2Widget = new QWidget;
    toolbar2Widget->setLayout(toolbar2Layout);
    toolbar2Widget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    toggleRenderBut = new QPushButton("rend");
    toggleSimulationBut = new QPushButton("sim");
    loadBut = new QPushButton("load");
    clearBut = new QPushButton("clr");
    saveBut = new QPushButton("save");
    dividingToolbar2But = new QPushButton("");
    dividingToolbar2But->setStyleSheet("QPushButton{background: transparent;}");

    toolbar2 = new QButtonGroup;
    toolbar2->addButton(toggleSimulationBut, 0);
    toolbar2->addButton(toggleRenderBut, 1);
    toolbar2->addButton(dividingToolbar2But);
    toolbar2->addButton(loadBut, 2);
    toolbar2->addButton(clearBut, 3);
    toolbar2->addButton(saveBut, 4);
    toolbar2->setExclusive(false);

    foreach(QAbstractButton* but, toolbar2->buttons())
    {
        but->setFixedSize(standartButtonToolbar2Size * 3, standartButtonToolbar2Size);
        toolbar2Layout->addWidget(but);
    }

    connect(toolbar2, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(myToolbar2Clicked(QAbstractButton*)));
}

// Создание кнопки и панели настроек.
void MainWindow::create_settings_widget()
{
    settingsBut = new QPushButton("set");
    settingsBut->setFixedSize(standartButtonToolbarSize, standartButtonToolbarSize);
    connect(settingsBut, SIGNAL(clicked(bool)), this, SLOT(settingsSlot()));
    settingsWidget = new SettingsWidget;
}

// Создание диалога выбора файла сцены.
void MainWindow::create_file_dialog()
{
    fileDialog = new QFileDialog();
    fileDialog->setDefaultSuffix("csv");
    fileDialog->setNameFilter("*.csv");
    connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(fileSelectedSlot(QString)));
}

MainWindow::~MainWindow()
{
}
