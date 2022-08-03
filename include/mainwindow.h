#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void resizeEvent(QResizeEvent *event);
    void test();

    GuitarBoard *guitarboard();

private:
    Ui::MainWindow *_ui;
};
#endif // MAINWINDOW_H
