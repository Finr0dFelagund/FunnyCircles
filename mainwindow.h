#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QPointF>
#include <QString>
#include <QVector2D>
#include <QtGlobal>

class QAbstractButton;
class QButtonGroup;
class QFileDialog;
class QGraphicsLineItem;
class QGraphicsRectItem;
class QGridLayout;
class QHBoxLayout;
class QPushButton;
class QSlider;
class QTimer;
class QWidget;

class MyQGraphicsView;
class SettingsWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    MyQGraphicsView* graphicsView;
    QWidget *aboveGraphicsViewWidget;
    QGridLayout *mainLayout;
    QWidget *mainWidget;
    QGridLayout *toolbarLayout;
    QButtonGroup *toolbar;
    QWidget *toolbarWidget;
    QPushButton *addCellBut, *deleteCellBut, *toggleMoveableBut, *toggleCollisionsBut, *createConnectionBut, *removeConnectionBut, *stopBut, *removeAllConnectionsBut, *grabBut, *createTypeBut;
    QWidget *toolbar2Widget;
    QHBoxLayout *toolbar2Layout;
    QButtonGroup *toolbar2;
    QPushButton *toggleRenderBut, *toggleSimulationBut, *loadBut, *clearBut, *saveBut;

    quint8 standartButtonToolbarSize, standartButtonToolbar2Size;
    QPushButton* dividingToolbar2But;
    bool addMode, deleteMode, boundMode, unBoundMode, grabMode, stopMode, toggleMoveableMode, toggleCollisionsMode, removeAllBoundsMode;

    QSlider* speedSlider;
    QGraphicsRectItem* selectionZone;

    QPushButton *settingsBut;
    SettingsWidget *settingsWidget;
    bool isSettingsView;
    quint16 bufferHeight, bufferWidth;

    static MainWindow* myWindow;
    quint16 bufferCell;
    bool isSimulationStopped, isRenderStopped;
    QPoint bufferPosition;
    QVector2D bufferPosition2;
    bool bufferMoveAble;
    quint8 bufferTypeId;
    QGraphicsLineItem *speedLine;

    QFileDialog *fileDialog;
    bool saving, loading;

    QTimer* grabber, *selector, *speeder;

    void InitializeTypes();

public slots:
    void addCellButSlot();
    void deleteCellButSlot();
    void toggleMoveableButSlot();
    void toggleCollisionsButSlot();
    void createConnectionButSlot();
    void removeConnectionButSlot();
    void stopButSlot();
    void removeAllConnectionsButSlot();
    void grabButSlot();
    void createTypeButSlot();
    void cellInteracted(quint16 cell, quint8 mouseButton, quint8 event);
    void myToolbarClicked(QAbstractButton* button);
    void slotGrabberAlarm();
    void myToolbar2Clicked(QAbstractButton* button);
    void speedSliderSlot(int value);
    void sceneInteracted(quint8 button, quint8 event);
    void slotSelectorAlarm();
    void settingsSlot();
    void typeCreated();
    void cellChosenSlot(int id);
    void speedSettingSlot();
    void fileSelectedSlot(QString file);

private:
    void create_timers();
    void create_view();
    void create_layout();
    void create_speed_slider();
    void create_toolbar();
    void create_toolbar2();
    void create_settings_widget();
    void create_file_dialog();
    QPointF scene_position_from_cursor() const;
    QPointF scene_position_from_global(const QPoint& point) const;
    void reset_modes();
    void apply_common_cell_action(quint16 cell_index);
    void save_view_geometry();
    void suspend_view();
    void resume_view();
    void show_page(QWidget* page);
    void fit_window_to_page();
    void restore_main_page();
};

#endif // MAINWINDOW_H

