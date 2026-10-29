#include "mainwindow.h"

#include "globalvars.h"

MainWindow* MainWindow::myWindow;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), standartButtonToolbarSize(40), standartButtonToolbar2Size(30),
    addMode(0), deleteMode(0), boundMode(0), unBoundMode(0), grabMode(0), stopMode(0), toggleMoveableMode(0), toggleCollisionsMode(0), removeAllBoundsMode(0),
    selectionZone(0), isSettingsView(0), bufferCell(0), isSimulationStopped(1), isRenderStopped(0), speedLine(0), saving(0), loading(0)
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
    connect(graphicsView, SIGNAL(mouseInteractSignal(quint16,quint8,quint8)), this, SLOT(cellInteracted(quint16,quint8,quint8)));
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

    settingsBut = new QPushButton("set");
    settingsBut->setFixedSize(standartButtonToolbarSize, standartButtonToolbarSize);
    connect(settingsBut, SIGNAL(clicked(bool)), this, SLOT(settingsSlot()));
    settingsWidget = new SettingsWidget;

    fileDialog = new QFileDialog();
    fileDialog->setDefaultSuffix("csv");
    fileDialog->setNameFilter("*.csv");
    connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(fileSelectedSlot(QString)));

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

void MainWindow::createTypeButSlot()
{
    reset_modes();

    save_view_geometry();
    show_page(new TypeCreationWidget);
    QObject::connect(centralWidget(), SIGNAL(destroyed(QObject*)), this, SLOT(typeCreated()));
    fit_window_to_page();
}

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
        // Для файла настроек без описания взаимодействий векторы дополняются
        // значениями по умолчанию: взаимодействие отсутствует, число связей не ограничено.
        while(interDirect.size() < CellType::types.size() + 1)
        {
            interDirect.append(0);
        }
        while(connectNum.size() < CellType::types.size() + 2)
        {
            connectNum.append(-1);
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
    restore_main_page();
    resume_view();
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

MainWindow::~MainWindow()
{
}
