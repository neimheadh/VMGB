#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QVBoxLayout>
#include "guitarboard.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void openSettingWindow();
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);

    GuitarBoard *guitarboard();

private:
    QVBoxLayout *_contentLayout;
    GuitarBoard *_guitarboard;
    Ui::MainWindow *_ui;
    QSettings *_settings;
};
#endif // MAINWINDOW_H
