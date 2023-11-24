#include "mainwindow.h"

MainWindow* MainWindow::myWindow;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), standartButtonToolbarSize(40), standartButtonToolbar2Size(30),
    addMode(0), deleteMode(0), boundMode(0), unBoundMode(0), grabMode(0), stopMode(0), toggleMoveableMode(0), toggleCollisionsMode(0), removeAllBoundsMode(0),
    selectionZone(0), isSettingsView(0), bufferCell(0), isSimulationStopped(1), isRenderStopped(0), speedLine(0)
{
    grabber = new QTimer;
    connect(grabber, SIGNAL(timeout()), this, SLOT(slotGrabberAlarm()));

    selector = new QTimer;
    connect(selector, SIGNAL(timeout()), this, SLOT(slotSelectorAlarm()));

    speeder = new QTimer;
    connect(speeder, SIGNAL(timeout()), this, SLOT(speedSettingSlot()));

    myWindow = this;
    graphicsView = new MyQGraphicsView;
    connect(graphicsView, SIGNAL(signalInteract(quint8,quint8)), this, SLOT(sceneInteracted(quint8,quint8)));
    connect(graphicsView, SIGNAL(mouseInteractSignal(Cell*,quint8,quint8)), this, SLOT(cellInteracted(Cell*,quint8,quint8)));
    graphicsView->startRendering();

    mainLayout = new QGridLayout;
    mainWidget = new QWidget;


    aboveGraphicsViewWidget = new QWidget;
    aboveGraphicsViewWidget->setFixedSize(1, 30);
    aboveGraphicsViewWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));


    speedSlider = new QSlider(Qt::Horizontal);
    mainLayout->addWidget(speedSlider, 3, 1, 1, 2, Qt::AlignRight | Qt::AlignBottom);
    connect(speedSlider, SIGNAL(valueChanged(int)), this, SLOT(speedSliderSlot(int)));
    speedSlider->setMaximum(100);
    speedSlider->setMinimum(0);
    speedSlider->setSingleStep(1);
    speedSlider->setInvertedAppearance(1);
    speedSlider->setValue(50);
    speedSlider->setFixedSize(300, 30);



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



    toolbar2Layout = new QHBoxLayout;
    toolbar2Layout->setSpacing(0);
    toolbar2Layout->setContentsMargins(0, 0, 0, 0);
    toolbar2Layout->setContentsMargins(0, 0, 0 ,0);

    toolbar2Widget = new QWidget;
    toolbar2Widget->setLayout(toolbar2Layout);
    toolbar2Widget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    toggleRenderBut = new QPushButton("rend");
    toggleSimulationBut = new QPushButton("sim");
    fillBut = new QPushButton("3");
    clearBut = new QPushButton("clr");
    redBut = new QPushButton("5");
    dividingToolbar2But = new QPushButton("");
    dividingToolbar2But->setStyleSheet("QPushButton{background: transparent;}");


    toolbar2 = new QButtonGroup;
    toolbar2->addButton(toggleSimulationBut, 0);
    toolbar2->addButton(toggleRenderBut, 1);
    toolbar2->addButton(dividingToolbar2But);
    toolbar2->addButton(fillBut, 2);
    toolbar2->addButton(clearBut, 3);
    toolbar2->addButton(redBut, 4);

    foreach(QAbstractButton* but, toolbar2->buttons())
    {
        but->setFixedSize(standartButtonToolbar2Size * 3, standartButtonToolbar2Size);
        toolbar2Layout->addWidget(but);
    }

    connect(toolbar2, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(myToolbar2Clicked(QAbstractButton*)));



    settingsBut = new QPushButton("set");
    settingsBut->setFixedSize(standartButtonToolbarSize, standartButtonToolbarSize);
    connect(settingsBut, SIGNAL(clicked(bool)), this, SLOT(settingsSlot()));
    settingsWidget = new SettingsWidget;



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

void  MainWindow::addCellButSlot()
{
    addMode = 1;
    deleteMode = 0;
    boundMode = 0;
    unBoundMode = 0;
    grabMode = 0;
    stopMode = 0;
    toggleMoveableMode = 0;
    toggleCollisionsMode = 0;
    removeAllBoundsMode  = 0;

    bufferHeight = height();
    bufferWidth = width();
    graphicsView->stopSimulation();
    graphicsView->stopRendering();
    takeCentralWidget();
    setCentralWidget(new AddCellWidget);
    connect(centralWidget(), SIGNAL(typeChosenSignal(int)), this, SLOT(cellChosenSlot(int)));
    adjustSize();
    setMaximumSize(width(), height());
}

void  MainWindow::deleteCellButSlot()
{
    addMode = 0;
    deleteMode = 1;
    boundMode = 0;
    unBoundMode = 0;
    grabMode = 0;
    stopMode = 0;
    toggleMoveableMode = 0;
    toggleCollisionsMode = 0;
    removeAllBoundsMode = 0;
}

void MainWindow::toggleMoveableButSlot()
{
    addMode = 0;
    deleteMode = 0;
    boundMode = 0;
    unBoundMode = 0;
    grabMode = 0;
    stopMode = 0;
    toggleMoveableMode = 1;
    toggleCollisionsMode = 0;
    removeAllBoundsMode = 0;
}

void MainWindow::toggleCollisionsButSlot()
{
    addMode = 0;
    deleteMode = 0;
    boundMode = 0;
    unBoundMode = 0;
    grabMode = 0;
    stopMode = 0;
    toggleMoveableMode = 0;
    toggleCollisionsMode = 1;
    removeAllBoundsMode  = 0;
}

void MainWindow::createConnectionButSlot()
{
    addMode = 0;
    deleteMode = 0;
    boundMode = 1;
    unBoundMode = 0;
    grabMode = 0;
    stopMode = 0;
    toggleMoveableMode = 0;
    toggleCollisionsMode = 0;
    removeAllBoundsMode = 0;

    bufferCell = 0;
}

void MainWindow::removeConnectionButSlot()
{
    addMode = 0;
    deleteMode = 0;
    boundMode = 0;
    unBoundMode = 1;
    grabMode = 0;
    stopMode = 0;
    toggleMoveableMode = 0;
    toggleCollisionsMode = 0;
    removeAllBoundsMode  = 0;

    bufferCell = 0;
}

void MainWindow::stopButSlot()
{
    addMode = 0;
    deleteMode = 0;
    boundMode = 0;
    unBoundMode = 0;
    grabMode = 0;
    stopMode = 1;
    toggleMoveableMode = 0;
    toggleCollisionsMode = 0;
    removeAllBoundsMode  = 0;
}

void MainWindow::removeAllConnectionsButSlot()
{
    addMode = 0;
    deleteMode = 0;
    boundMode = 0;
    unBoundMode = 0;
    grabMode = 0;
    stopMode = 0;
    toggleMoveableMode = 0;
    toggleCollisionsMode = 0;
    removeAllBoundsMode  = 1;
}

void MainWindow::grabButSlot()
{
    addMode = 0;
    deleteMode = 0;
    boundMode = 0;
    unBoundMode = 0;
    grabMode = 1;
    stopMode = 0;
    toggleMoveableMode = 0;
    toggleCollisionsMode = 0;
    removeAllBoundsMode  = 0;
}

void MainWindow::createTypeButSlot()
{
    addMode = 0;
    deleteMode = 0;
    boundMode = 0;
    unBoundMode = 0;
    grabMode = 0;
    stopMode = 0;
    toggleMoveableMode = 0;
    toggleCollisionsMode = 0;
    removeAllBoundsMode  = 0;

    bufferHeight = height();
    bufferWidth = width();
    graphicsView->stopSimulation();
    graphicsView->stopRendering();
    takeCentralWidget();
    setCentralWidget(new TypeCreationWidget);
    QObject::connect(centralWidget(), SIGNAL(destroyed(QObject*)), this, SLOT(typeCreated()));
    adjustSize();
    setMaximumSize(width(), height());
}

/*void MainWindow::cellInteracted(Cell* cell, quint8 mouseButton, quint8 event)
{
    if(mouseButton == 0)
    {
        if(event == 0)
        {
            if(deleteMode && QApplication::keyboardModifiers() != Qt::ShiftModifier)
            {
                delete cell;
            }
            else if(stopMode)
            {
                cell->setSpeed(QVector2D(0, 0));
            }
            else if(toggleMoveableMode)
            {
                if(cell->getMoveAble())
                {
                    cell->setMoveAble(0);
                }
                else
                {
                    cell->setMoveAble(1);
                }
            }
            else if(toggleCollisionsMode)
            {
                if(cell->getCollisionAble())
                {
                    cell->setCollisionAble(0);
                }
                else
                {
                    cell->setCollisionAble(1);
                }
            }
            else if(removeAllBoundsMode)
            {
                cell->removeAllConnections();
            }
            else if(boundMode)
            {
                if(bufferCell == 0)
                {
                    bufferCell = cell;
                }
                else
                {
                    Cell::tryConnect(bufferCell, cell);
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
                    Cell::removeConnect(bufferCell, cell);
                    bufferCell = 0;
                }
            }
            else if(grabMode)
            {
                bufferCell = cell;
                grabber->start(5);
                bufferPosition = QCursor::pos();
                bufferPosition2 = cell->getPosition();
                bufferMoveAble = cell->getMoveAble();
                cell->setMoveAble(0);
            }
        }
        else if(event == 1)
        {
            if(grabMode)
            {
                grabber->stop();
                cell->setMoveAble(bufferMoveAble);
            }
        }
    }
}*/

void MainWindow::slotGrabberAlarm()
{
    QVector2D newPos = bufferPosition2;
    newPos.setX(newPos.x() + (QCursor::pos().x() - bufferPosition.x()));
    newPos.setY(newPos.y() + (QCursor::pos().y() - bufferPosition.y()));
    graphicsView->cells.cells[bufferCell]->position = newPos;
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
        }
        else
        {
            graphicsView->stopSimulation();
            isSimulationStopped = 1;
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
    else if(button == fillBut)
    {
        ;
    }
    else if(button == clearBut)
    {
        while(graphicsView->cells.cells.size())
        {
            graphicsView->deleteCell(0);
        }
    }
    else if(button == redBut)
    {
        ;
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

void MainWindow::sceneInteracted(quint8 button, quint8 event)
{
    if(button == 0)
    {
        if(event == 0)
        {
            if(!addMode)
            {
                selectionZone = new QGraphicsRectItem(QRectF(graphicsView->mapToScene(graphicsView->mapFromGlobal(QCursor::pos()).x(), graphicsView->mapFromGlobal(QCursor::pos()).y()), graphicsView->mapToScene(graphicsView->mapFromGlobal(QCursor::pos()).x() + 1, graphicsView->mapFromGlobal(QCursor::pos()).y() + 1)));
                selectionZone->setBrush(Qt::Dense7Pattern);
                graphicsView->scene.addItem(selectionZone);
                bufferPosition = QCursor::pos();
                selector->start(10);
            }
            else
            {
                bufferPosition.setX(graphicsView->mapToScene(graphicsView->mapFromGlobal(QCursor::pos()).x(), graphicsView->mapFromGlobal(QCursor::pos()).y()).x());
                bufferPosition.setY(graphicsView->mapToScene(graphicsView->mapFromGlobal(QCursor::pos()).x(), graphicsView->mapFromGlobal(QCursor::pos()).y()).y());
                graphicsView->addCell(CellType::types[bufferTypeId], QVector2D(bufferPosition.x(), bufferPosition.y()), QVector2D(0, 0));
                graphicsView->cells.cells.last()->moveAble = false;
                graphicsView->cells.cells.last()->collisionAble = false;
                //bufferCell = graphicsView->cells.cells.size() - 1;
                //bufferCell->setMoveAble(0);
                //bufferCell->setCollisionAble(0);
                speedLine = new QGraphicsLineItem(bufferPosition.x(), bufferPosition.y(), bufferPosition.x()+1, bufferPosition.y()+1);
                graphicsView->scene.addItem(speedLine);
                speeder->start(10);
            }
        }
        else if(event == 1)
        {
            /*if(selectionZone != 0)
            {
                foreach(QGraphicsItem* item, selectionZone->collidingItems(Qt::IntersectsItemShape))
                {
                    if(item->data(0).toString() == "cell")
                    {
                        if(deleteMode)
                        {
                            delete static_cast<Cell*>(item);
                        }
                        else if(stopMode)
                        {
                            static_cast<Cell*>(item)->setSpeed(QVector2D(0, 0));
                        }
                        else if(toggleMoveableMode)
                        {
                            if(static_cast<Cell*>(item)->getMoveAble())
                            {
                                static_cast<Cell*>(item)->setMoveAble(0);
                            }
                            else
                            {
                                static_cast<Cell*>(item)->setMoveAble(1);
                            }
                        }
                        else if(toggleCollisionsMode)
                        {
                            if(static_cast<Cell*>(item)->getCollisionAble())
                            {
                                static_cast<Cell*>(item)->setCollisionAble(0);
                            }
                            else
                            {
                                static_cast<Cell*>(item)->setCollisionAble(1);
                            }
                        }
                        else if(removeAllBoundsMode)
                        {
                            static_cast<Cell*>(item)->removeAllConnections();
                        }
                    }
                }
                selector->stop();
                delete selectionZone;
                selectionZone = 0;
            }*/

            if(speedLine != 0)
            {
                speeder->stop();
                delete speedLine;
                speedLine = 0;
                graphicsView->cells.cells.last()->moveAble = true;
                graphicsView->cells.cells.last()->collisionAble = true;
                graphicsView->cells.cells.last()->speed = QVector2D(graphicsView->mapToScene(graphicsView->mapFromGlobal(QCursor::pos()).x(), graphicsView->mapFromGlobal(QCursor::pos()).y()).x() - bufferPosition.x(), graphicsView->mapToScene(graphicsView->mapFromGlobal(QCursor::pos()).x(), graphicsView->mapFromGlobal(QCursor::pos()).y()).y() - bufferPosition.y());
                //bufferCell->setMoveAble(1);
                //bufferCell->setCollisionAble(1);
                //bufferCell->setSpeed(QVector2D(graphicsView->mapToScene(graphicsView->mapFromGlobal(QCursor::pos()).x(), graphicsView->mapFromGlobal(QCursor::pos()).y()).x() - bufferPosition.x(), graphicsView->mapToScene(graphicsView->mapFromGlobal(QCursor::pos()).x(), graphicsView->mapFromGlobal(QCursor::pos()).y()).y() - bufferPosition.y()));
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
    if(QCursor::pos().x() >= bufferPosition.x() && QCursor::pos().y() >= bufferPosition.y())
    {
        selectionZone->setRect(QRectF(graphicsView->mapToScene(graphicsView->mapFromGlobal(bufferPosition).x(), graphicsView->mapFromGlobal(bufferPosition).y()), graphicsView->mapToScene(graphicsView->mapFromGlobal(QCursor::pos()).x(), graphicsView->mapFromGlobal(QCursor::pos()).y())));
    }
    else if(QCursor::pos().x() >= bufferPosition.x() && QCursor::pos().y() < bufferPosition.y())
    {
        selectionZone->setRect(QRectF(graphicsView->mapToScene(graphicsView->mapFromGlobal(bufferPosition).x(), graphicsView->mapFromGlobal(QCursor::pos()).y()), graphicsView->mapToScene(graphicsView->mapFromGlobal(QCursor::pos()).x(), graphicsView->mapFromGlobal(bufferPosition).y())));
    }
    else if(QCursor::pos().x() < bufferPosition.x() && QCursor::pos().y() >= bufferPosition.y())
    {
        selectionZone->setRect(QRectF(graphicsView->mapToScene(graphicsView->mapFromGlobal(QCursor::pos()).x(), graphicsView->mapFromGlobal(bufferPosition).y()), graphicsView->mapToScene(graphicsView->mapFromGlobal(bufferPosition).x(), graphicsView->mapFromGlobal(QCursor::pos()).y())));
    }
    else if(QCursor::pos().x() < bufferPosition.x() && QCursor::pos().y() < bufferPosition.y())
    {
        selectionZone->setRect(QRectF(graphicsView->mapToScene(graphicsView->mapFromGlobal(QCursor::pos()).x(), graphicsView->mapFromGlobal(QCursor::pos()).y()), graphicsView->mapToScene(graphicsView->mapFromGlobal(bufferPosition).x(), graphicsView->mapFromGlobal(bufferPosition).y())));
    }
}

void MainWindow::settingsSlot()
{
    if(isSettingsView)
    {
        isSettingsView = 0;
        setMaximumSize(2048, 2048);
        takeCentralWidget();
        setCentralWidget(mainWidget);
        mainLayout->addWidget(settingsBut, 0, 2, 1, 1, Qt::AlignTop | Qt::AlignRight);
        resize(bufferWidth, bufferHeight);
        setMinimumWidth(standartButtonToolbar2Size * 3 * 6 + 300 + 60);
        if(GLOBALVARS::simulationSpeed == GLOBALVARS::slowModeRait)
        {
            speedSlider->hide();
        }
        else
        {
            speedSlider->show();
            GLOBALVARS::simulationSpeed = speedSlider->value();
        }
        if(isSimulationStopped == 0)
        {
            graphicsView->startSimulation();
        }
        if(isRenderStopped == 0)
        {
            graphicsView->startRendering();
        }
    }
    else
    {
        isSettingsView = 1;
        bufferHeight = height();
        bufferWidth = width();
        takeCentralWidget();
        graphicsView->stopSimulation();
        graphicsView->stopRendering();
        setCentralWidget(settingsWidget);
        settingsWidget->layout->addWidget(settingsBut, 0, 1, 1, 1, Qt::AlignTop | Qt::AlignRight);
        adjustSize();
        setMaximumSize(width(), height());
    }
}

void MainWindow::typeCreated()
{
    setMaximumSize(2048, 2048);
    setCentralWidget(mainWidget);
    mainLayout->addWidget(settingsBut, 0, 2, 1, 1, Qt::AlignTop | Qt::AlignRight);
    resize(bufferWidth, bufferHeight);
    setMinimumWidth(standartButtonToolbar2Size * 3 * 6 + 300 + 60);
    if(isSimulationStopped == 0)
    {
        graphicsView->startSimulation();
    }
    if(isRenderStopped == 0)
    {
        graphicsView->startRendering();
    }
    myToolbar2Clicked(clearBut);
}

void MainWindow::InitializeTypes()
{
    QSettings *settings = new QSettings("settings.ini", QSettings::IniFormat);
    settings->beginGroup("Types");
    quint8 number = settings->value("number").toInt();
    settings->beginReadArray("types");
    for(quint8 i = 0; i < number; i++)
    {
        settings->setArrayIndex(i);
        QList<QVariant> buff = settings->value("interactDirection").toList();
        QVector<qint8> interDirect;
        QVector<qint8> connectNum;
        while(buff.count() != 0)
        {
            interDirect.append(buff[0].toInt());
            buff.removeFirst();
        }
        buff = settings->value("maxConnectionNumber").toList();
        while(buff.count() != 0)
        {
            connectNum.append(buff[0].toInt());
            buff.removeFirst();
        }
        CellType *ctp = new CellType(QBrush(QColor(settings->value("color1").toInt(), settings->value("color2").toInt(), settings->value("color3").toInt())), &GLOBALVARS::force, interDirect, settings->value("size").toReal(), settings->value("mass").toReal(), settings->value("friction").toReal(), settings->value("hardness").toReal(), connectNum);
        ctp->name = settings->value("name").toString();
    }
    settings->endArray();
    settings->endGroup();
    delete settings;
}

void MainWindow::cellChosenSlot(int id)
{
    bufferTypeId = (id == -1) ? bufferTypeId : id;
    setMaximumSize(2048, 2048);
    setCentralWidget(mainWidget);
    mainLayout->addWidget(settingsBut, 0, 2, 1, 1, Qt::AlignTop | Qt::AlignRight);
    resize(bufferWidth, bufferHeight);
    setMinimumWidth(standartButtonToolbar2Size * 3 * 6 + 300 + 60);
    if(isSimulationStopped == 0)
    {
        graphicsView->startSimulation();
    }
    if(isRenderStopped == 0)
    {
        graphicsView->startRendering();
    }
}

void MainWindow::speedSettingSlot()
{
    speedLine->setLine(bufferPosition.x(), bufferPosition.y(), graphicsView->mapToScene(graphicsView->mapFromGlobal(QCursor::pos()).x(), graphicsView->mapFromGlobal(QCursor::pos()).y()).x(), graphicsView->mapToScene(graphicsView->mapFromGlobal(QCursor::pos()).x(), graphicsView->mapFromGlobal(QCursor::pos()).y()).y());
}

MainWindow::~MainWindow()
{
}
