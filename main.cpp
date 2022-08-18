#include <iostream>
#include <thread>
#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "eventmanager.h"
#include "mainwindow.h"
#include "midi/manager.h"
#include "guitar/board.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    for (const QString &locale : uiLanguages) {
        const QString baseName = "VMGB_" + QLocale(locale).name();
        if (translator.load(baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    EventManager eventManager;
    MIDI::Manager mid(&eventManager);
    MainWindow w(&eventManager);

    mid.start();
    w.show();
    int errcode = a.exec();
    mid.end();

    return errcode;
}
