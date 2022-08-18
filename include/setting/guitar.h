#ifndef GUITARSETTING_H
#define GUITARSETTING_H

#include <QLabel>
#include <QSettings>
#include <QSpinBox>
#include <QTextEdit>
#include <QWidget>
#include "eventmanager.h"
#include "setting/midinoteinput.h"

namespace Ui {
class SettingGuitar;
}

namespace Setting {
    class Guitar : public QWidget
    {
        Q_OBJECT

    public:
        /**
         * @brief Default frets count.
         */
        static const unsigned char DEFAULT_FRETS = 24;
        /**
         * @brief Default strings count.
         */
        static const unsigned char DEFAULT_STRINGS = 6;
        /**
         * @brief Default tuning.
         */
        static const unsigned char DEFAULT_TUNING[];
        /**
         * @brief Sended save event name.
         */
        static const char EVENT_SAVE[];
        /**
         * @brief Frets setting name.
         */
        static const char SETTING_FRETS[];
        /**
         * @brief Strings setting name.
         */
        static const char SETTING_STRINGS[];
        /**
         * @brief Tuning setting name.
         */
        static const char SETTING_TUNING[];
        /**
         * @brief Guitar application settings group.
         */
        static const char SETTINGS_GROUP[];

        static QString tuningToSetting(unsigned char strings, const unsigned char *tuning);
        static unsigned char *settingToTuning(QString setting);
        static unsigned char *settingToTuning(QVariant setting);

        explicit Guitar(QSettings *settings, EventManager *eventManager = nullptr, QWidget *parent = nullptr);
        ~Guitar();

        void save();

        void showEvent(QShowEvent *event);
        unsigned char frets();
        static unsigned char frets(QSettings *settings);
        unsigned char strings();
        static unsigned char strings(QSettings *settings);
        const unsigned char *tuning();
        static unsigned char *tuning(QSettings *settings);

    private slots:
        void on_frets_input_valueChanged(int frets);
        void on_strings_input_valueChanged(int strings);

    private:
        EventManager *_eventManager;
        Ui::SettingGuitar *_ui;
        QSettings *_settings;

        bool _changed = false;
        unsigned char _frets;
        unsigned char _strings;
        unsigned char *_tuning = nullptr;

        MidiNoteInput **_tuningInputs = nullptr;
        QLabel **_tuningLabels = nullptr;

        QSpinBox *_fretsInput();
        QSpinBox *_stringsInput();
        QWidget *_tuningArea();

        void _buildTuning();
        void _clearTuning();
        void _loadSetting();
    };
}

#endif // GUITARSETTING_H
