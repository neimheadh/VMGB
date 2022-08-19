#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QVBoxLayout>
#include "eventmanager.h"
#include "guitar/board.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QSettings *settings, EventManager *eventManager = nullptr, QWidget *parent = nullptr);
    ~MainWindow();

    void openSettingsWindow();
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);

    Guitar::Board *guitarboard();

private:
    QVBoxLayout *_contentLayout;
    EventManager *_eventManager;
    Guitar::Board *_guitarboard;
    Ui::MainWindow *_ui;
    QSettings *_settings;
};
#endif // MAINWINDOW_H
