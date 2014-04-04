#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QTimer>

#include "videocontroller.h"
#include "infofile.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void paintEvent(QPaintEvent *);

    void resizeEvent(QResizeEvent *);

    void keyPressEvent(QKeyEvent *event);

protected:
    void nextFrames();

private slots:
    void on_actionOpen_avi_triggered();

    void on_horizontalSlider_sliderMoved(int position);

    void on_pushButton_clicked();

    void on_updateFrame();

private:
    Ui::MainWindow *ui;

    VideoController m_controller;

    InfoFile m_info;

    QTimer *m_timer;
};

#endif // MAINWINDOW_H
