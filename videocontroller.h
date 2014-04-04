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

    void setFramePosition(int pos);

    cv::Mat getFrame();

    int getCurentFrameNum();

    int getTotalFrameCount();

    void drawRectangle(QRect& rect);

    bool saveRectangle();

private:
    cv::Mat m_frame;

    int m_curFrameNum;

    cv::VideoCapture m_capture;

    QVector<QRect> m_rects;

signals:

public slots:

};

#endif // VIDEOCONTROLLER_H
