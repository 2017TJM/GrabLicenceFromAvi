#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QPointF>

#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_controller(this)
{
    ui->setupUi(this);

    m_timer = new QTimer(this);
    m_timer->setInterval(20);
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
    int w = m_controller.getWidth();
    int h = m_controller.getHeight();
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
        on_pushButton_clicked();
        break;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button())
    {
        QPointF localPos = event->localPos();
//        int mbHeight = ui->menuBar->height();
        qDebug() << localPos.x() << " " << localPos.y()/* - mbHeight*/;

        m_grabRect.setLeft(localPos.x());
        m_grabRect.setTop(localPos.y()/* - mbHeight*/);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPointF localPos = event->localPos();
//    int mbHeight = ui->menuBar->height();
    qDebug() << localPos.x() << " " << localPos.y()/* - mbHeight*/;
    m_grabRect.setRight(localPos.x());
    m_grabRect.setBottom(localPos.y()/* - mbHeight*/);

    m_controller.beginDraw();
    cv::Mat img = m_controller.getFrame();

    cv::Rect rec;

    rec = windowToFrame(m_grabRect);

    cv::rectangle(img, rec, cv::Scalar(255, 0, 0), 5);
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button())
    {
        m_controller.drawRectangle(windowToFrame(m_grabRect));
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta();

    if (numDegrees.y() < 0)
    {
        int pos = m_controller.getCurentFrameNum();
        m_controller.setFramePosition(pos + 50);
        nextFrames();
    }
    else
    {
        preFrames();
    }

    update();
}

void MainWindow::nextFrames()
{
    int curNum = m_controller.getCurentFrameNum();
    int totalCount = m_controller.getTotalFrameCount();

    ui->label->setText(QString().setNum(curNum)
                       + "/" + QString().setNum(totalCount));

    ui->horizontalSlider->setSliderPosition(curNum);

    if( !m_controller.getNextFrame())
    {
        qDebug() << "Can't read frame";
        return;
    }
}

void MainWindow::preFrames()
{
    m_controller.getPreFrame();

    int curNum = m_controller.getCurentFrameNum();
    int totalCount = m_controller.getTotalFrameCount();

    ui->label->setText(QString().setNum(curNum)
                       + "/" + QString().setNum(totalCount));

    ui->horizontalSlider->setSliderPosition(curNum);

    if( !m_controller.getNextFrame())
    {
        qDebug() << "Can't read frame";
        return;
    }
}

cv::Rect MainWindow::windowToFrame(QRect &rect)
{
    int w = m_controller.getWidth();
    int h = m_controller.getHeight();
    int sw = width();
    int sh = height();
    float wr = float(w) / float(sw);
    float hr = float(h) / float(sh);

    int tx = rect.x() * wr;
    int ty = rect.y() * hr;
    int tw = rect.width() * wr;
    int th = rect.height() * hr;

    return cv::Rect(tx, ty, tw, th);
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

    int w = m_controller.getWidth();
    int h = m_controller.getWidth();
    float ratio = (float)h / (float)w;
    int sw = width();
    int sh = sw * ratio;

    resize(sw, sh);

    InfoFile *info = InfoFile::getInstance();

    info->open();
    *info << "////////////////////////////" << "\n";
    *info << path << "\n";
    *info << "\n";
    info->close();

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
    int pos = m_controller.getCurentFrameNum();
    ui->horizontalSlider->setSliderPosition(pos);
    update();
}
