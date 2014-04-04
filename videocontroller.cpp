#include "videocontroller.h"
#include <QDebug>

VideoController::VideoController(QObject *parent) :
    QObject(parent),
    m_curFrameNum(0)
{
}

bool VideoController::openVideo(QString videoname)
{
    if(videoname.isEmpty())
        return false;

    m_capture.open(videoname.toStdString());

    if (!m_capture.isOpened())
    {
        qDebug() << "video can't open";
        return false;
    }

    return true;
}

bool VideoController::isOpen()
{
    return m_frame.data;
}

bool VideoController::getNextFrame()
{
    if (!m_capture.read(m_frame))
    {
        return false;
    }

    m_curFrameNum++;

    return true;

}

void VideoController::setFramePosition(int pos)
{
    m_capture.set(CV_CAP_PROP_POS_FRAMES, pos);
    m_curFrameNum = pos;
}

cv::Mat VideoController::getFrame()
{
    return m_frame;
}

int VideoController::getCurentFrameNum()
{
    return m_curFrameNum;
}

int VideoController::getTotalFrameCount()
{
    return (int)m_capture.get(CV_CAP_PROP_FRAME_COUNT);
}

