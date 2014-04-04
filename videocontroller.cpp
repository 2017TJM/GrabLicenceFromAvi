#include "videocontroller.h"
#include "infofile.h"
#include <QDebug>

VideoController::VideoController(QObject *parent) :
    QObject(parent),
    m_curFrameNum(0),
    m_isDraw(false)
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
    if (m_isDraw)
    {
        saveRectangle();
        imwrite(QString(".//frames/" + QString().setNum(getCurentFrameNum())
                        + ".jpg").toStdString(), m_frame);
    }

    if (!m_capture.read(m_frame))
    {
        return false;
    }

    m_curFrameNum++;

    m_isDraw = false;

    m_rects.clear();

    return true;

}

bool VideoController::getPreFrame()
{
    if (m_isDraw)
    {
        saveRectangle();
        imwrite(QString(".//frames/" + QString().setNum(getCurentFrameNum())
                        + ".jpg").toStdString(), m_frame);
    }

    m_capture.set(CV_CAP_PROP_POS_FRAMES, m_curFrameNum-50);

    if (!m_capture.read(m_frame))
    {
        return false;
    }

    m_curFrameNum -= 50;

    m_isDraw = false;

    m_rects.clear();

    return true;
}

void VideoController::setFramePosition(int pos)
{
    m_capture.set(CV_CAP_PROP_POS_FRAMES, pos);
    m_curFrameNum = pos;
}

cv::Mat VideoController::getFrame()
{
    cv::Mat result;
    if (m_isDraw)
    {
        if (!m_rects.isEmpty())
        {
            QVector<cv::Rect>::iterator iter = m_rects.begin();
            QVector<cv::Rect>::iterator iterEnd = m_rects.end();
            for (; iter != iterEnd; ++iter)
            {
                cv::rectangle(m_drawFrame, *iter, cv::Scalar(255, 0, 0), 5);
            }
        }

        result = m_drawFrame;
    }
    else
        result = m_frame;

    return result;
}

int VideoController::getWidth()
{
    return m_frame.cols;
}

int VideoController::getHeight()
{
    return m_frame.rows;
}

void VideoController::beginDraw()
{
    m_frame.copyTo(m_drawFrame);
    m_isDraw = true;
}

int VideoController::getCurentFrameNum()
{
    return m_curFrameNum;
}

int VideoController::getTotalFrameCount()
{
    return (int)m_capture.get(CV_CAP_PROP_FRAME_COUNT);
}

void VideoController::drawRectangle(cv::Rect &rect)
{
    m_rects.push_back(rect);
}

bool VideoController::saveRectangle()
{
    QString folder = "grab_plates/";
    QString file = QString().setNum(m_curFrameNum);
    cv::Mat plate;
    int no = 0;

    InfoFile* info = InfoFile::getInstance();
    info->open();

    *info << folder << file << ".jpg" << " 1";

    QVector<cv::Rect>::iterator iter = m_rects.begin();
    QVector<cv::Rect>::iterator iterEnd = m_rects.end();
    for (; iter != iterEnd; ++iter)
    {
        plate = m_frame(*iter);
        QString filename = ".//" + folder + file + "_"
                + QString().setNum(no++) + ".jpg";
        cv::imwrite(filename.toStdString(), plate);

        *info << " " << iter->x << " " << iter->y << " " << iter->width << " " << iter->height;

    }

    *info << "\n";
    info->close();

    return true;
}

