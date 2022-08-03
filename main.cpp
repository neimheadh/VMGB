#include <iostream>
#include <thread>
#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "mainwindow.h"
#include "midi.h"
#include "guitarboard.h"

void on_midi_event(struct MidiEvent event, MainWindow *w)
{
    switch (event.type) {
    case MidiEventType::NOTE_OFF:
        w->guitarboard()->releaseMidi(event.value);
        break;
    case MidiEventType::NOTE_ON:
        w->guitarboard()->playMidi(event.value);
        break;
    }
}

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

    Midi mid;
    MainWindow w;

    mid.onMidiEvent((midi_handler_t) &on_midi_event, &w);
    mid.start();
    w.show();
    int errcode = a.exec();
    mid.end();

    return errcode;
}