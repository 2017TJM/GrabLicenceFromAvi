#-------------------------------------------------
#
# Project created by QtCreator 2014-04-03T10:22:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GrabLicenceFromAvi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    videocontroller.cpp \
    infofile.cpp

HEADERS  += mainwindow.h \
    videocontroller.h \
    infofile.h

FORMS    += mainwindow.ui

INCLUDEPATH += C:\OpenCV-2.2.0-win\OpenCV-2.2.0\include\opencv2 \
    C:\OpenCV-2.2.0-win\OpenCV-2.2.0\include \
    C:\OpenCV-2.2.0-win\OpenCV-2.2.0\include\opencv

LIBS+= C:\OpenCV-2.2.0-win\OpenCV-2.2.0\bulid\lib\debug\opencv_core220d.lib \
C:\OpenCV-2.2.0-win\OpenCV-2.2.0\bulid\lib\debug\opencv_imgproc220d.lib \
C:\OpenCV-2.2.0-win\OpenCV-2.2.0\bulid\lib\debug\opencv_highgui220d.lib \
C:\OpenCV-2.2.0-win\OpenCV-2.2.0\bulid\lib\debug\opencv_ml220d.lib \
C:\OpenCV-2.2.0-win\OpenCV-2.2.0\bulid\lib\debug\opencv_video220d.lib \
C:\OpenCV-2.2.0-win\OpenCV-2.2.0\bulid\lib\debug\opencv_features2d220d.lib \
C:\OpenCV-2.2.0-win\OpenCV-2.2.0\bulid\lib\debug\opencv_calib3d220d.lib \
C:\OpenCV-2.2.0-win\OpenCV-2.2.0\bulid\lib\debug\opencv_objdetect220d.lib \
C:\OpenCV-2.2.0-win\OpenCV-2.2.0\bulid\lib\debug\opencv_contrib220d.lib \
C:\OpenCV-2.2.0-win\OpenCV-2.2.0\bulid\lib\debug\opencv_legacy220d.lib \
C:\OpenCV-2.2.0-win\OpenCV-2.2.0\bulid\lib\debug\opencv_flann220d.lib
