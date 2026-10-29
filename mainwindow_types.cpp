#include "mainwindow.h"

#include <QBrush>
#include <QColor>
#include <QList>
#include <QSettings>
#include <QVariant>

#include "celltype.h"
#include "globalvars.h"

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
