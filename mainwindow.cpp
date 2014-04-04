#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QPainter>
#include <QTimer>

#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_controller(this),
    m_info("./info.txt", this)
{
    ui->setupUi(this);

    m_timer = new QTimer(this);
    m_timer->setInterval(1);
    m_timer->start();
}

MainWindow::~MainWindow()
{
    m_timer->stop();
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if (m_controller.isOpen())
    {
        cv::Mat img = m_controller.getFrame();

        int sw = width();
        int sh = height();

        cv::Mat temp;
        cv::cvtColor(img, temp, CV_BGR2RGB);
        QImage image = QImage(temp.data, img.cols, img.rows,
                              temp.cols * temp.channels(),
                              QImage::Format_RGB888);

        QRectF rect(0, 0, sw, sh);
        painter.drawImage(rect, image);
    }
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    cv::Mat img = m_controller.getFrame();
    int w = img.cols;
    int h = img.rows;
    float ratio = (float)h / (float)w;
    int sw = width();
    int sh = height();
    float ratio1 = (float)sh / (float)sw;

    if (ratio != ratio1)
    {
        resize(sw, ratio * sw);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    switch( key )
    {
    case Qt::Key_Space:
        nextFrames();
        update();
        break;
    }
}

void MainWindow::nextFrames()
{
    int curNum = m_controller.getCurentFrameNum();
    int totalCount = m_controller.getTotalFrameCount();

    ui->label->setText(QString().setNum(curNum)
                       + "/" + QString().setNum(totalCount));

    if( !m_controller.getNextFrame())
    {
        qDebug() << "Can't read frame";
        return;
    }
}

void MainWindow::on_actionOpen_avi_triggered()
{
    QFileDialog fd;

    QString path = fd.getOpenFileName(this, "Open avi file",
                       ".");

    if (path.isEmpty())
    {
        QMessageBox::warning(this, "error", "Can't open this avi");
        return ;
    }

    if( !m_controller.openVideo(path))
    {
        qDebug() << "Can't open frame";
        return;
    }

    int totalCount = m_controller.getTotalFrameCount();
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(totalCount);

    nextFrames();

    cv::Mat img = m_controller.getFrame();
    int w = img.cols;
    int h = img.rows;
    float ratio = (float)h / (float)w;
    int sw = width();
    int sh = sw * ratio;

    resize(sw, sh);

    m_info.open();
    m_info << "////////////////////////////" << "\n";
    m_info << path << "\n";
    m_info << "\n";

    update();
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    m_controller.setFramePosition(position);

    nextFrames();

    update();
}

void MainWindow::on_pushButton_clicked()
{
    static bool run = false;
    if (true == run)
    {
        disconnect(m_timer, SIGNAL(timeout()), this, SLOT(on_updateFrame()));
        run = false;
        ui->pushButton->setText("Start");
    }
    else
    {
        connect(m_timer, SIGNAL(timeout()), this, SLOT(on_updateFrame()));
        run = true;
        ui->pushButton->setText("Stop");
    }
}

void MainWindow::on_updateFrame()
{
    nextFrames();
    update();
}
