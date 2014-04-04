#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include <QObject>
#include <QString>
#include <QRect>
#include <QVector>
#include <opencv2/opencv.hpp>

class VideoController : public QObject
{
    Q_OBJECT
public:
    explicit VideoController(QObject *parent = 0);

    bool openVideo(QString videoname);

    bool isOpen();

    bool getNextFrame();

    bool getPreFrame();

    void setFramePosition(int pos);

    cv::Mat getFrame();

    int getWidth();

    int getHeight();

    void beginDraw();

    int getCurentFrameNum();

    int getTotalFrameCount();

    void drawRectangle(cv::Rect &rect);

    bool saveRectangle();

private:
    cv::Mat m_frame;

    cv::Mat m_drawFrame;

    int m_curFrameNum;

    cv::VideoCapture m_capture;

    QVector<cv::Rect> m_rects;

    bool m_isDraw;

signals:

public slots:

};

#endif // VIDEOCONTROLLER_H
