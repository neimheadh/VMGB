#ifndef MIDINOTEINPUT_H
#define MIDINOTEINPUT_H

#include <QLineEdit>
#include <QString>
#include <QWidget>
#include <map>

namespace Ui {
class MidiNoteInput;
}

namespace Setting {

class MidiNoteInput : public QWidget
{
    Q_OBJECT

public:
    explicit MidiNoteInput(QWidget *parent = nullptr);
    explicit MidiNoteInput(unsigned char note, QWidget *parent = nullptr);
    ~MidiNoteInput();

    void keyPressEvent(QKeyEvent *event);
    void showEvent(QShowEvent *event);

    QLineEdit *input();

    void setValue(unsigned char note);
    unsigned char value();

signals:
    void valueChanged(MidiNoteInput *input, unsigned char newValue);

private slots:
    void on_increase_clicked();

    void on_decrease_clicked();

private:
    Ui::MidiNoteInput *_ui;
    unsigned char _value = 0;
};

}

#endif // MIDINOTEINPUT_H
